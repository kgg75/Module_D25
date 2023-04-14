// ������� ��� ����������� � ���� ������, �������� ������, �������� ���� ������
#include "StdAfx.h"

#include "DBFunctions.h"
#include "Constants.h"
#include "Users.h"
#include "Messages.h"
#include "ServiceFunc.h"


using namespace std;


SQLHENV     sqlEnvHandle {nullptr}; // ���������� ��������� ���� ������
SQLHDBC     sqlConnHandle {nullptr}; // ���������� ��� ���������� � ����� ������
SQLHSTMT    sqlStmtHandle {nullptr};  // ���������� ��� ���������� �������� � ���� ������
SQLCHAR     retconstring[SQL_RETURN_CODE_LEN] {}; // ������ ��� ���� �������� �� ������� API ODBC
SQLRETURN   sqlResult;


// ���������� ��� ������� SQLGetDiagRecA - ���������� ������� �������� ���������� ����� ��������������� ������,
// ���������� �������� �� ������, �������������� � ���������.
#if _DEBUG 
    SQLCHAR		SQLState;
    SQLINTEGER	NativeError;
    SQLCHAR		ErrorMessageText[SQL_BUFFER_LENGTH];
    SQLSMALLINT	ErrorTextLength;
#endif


// simple helper functions  
inline int MySQLSuccess(SQLRETURN rc) { return (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO); }


// ��������� ������������� ������, � ������ ���������� ������ ��
int CheckTables() {
    cout << "�������� ������� �������������...";

    if (MySQLSuccess(SQLExecDirectA(sqlStmtHandle, BuildStatement("SELECT * FROM ", USERS_TABLE_NAME, " LIMIT 1"), SQL_NTS))) {
        cout << " ���������.\n";
    } else {
        cout << "\n��������������: ������� ������������� �����������!\n";
        if (MySQLSuccess(sqlResult = SQLExecDirectA(sqlStmtHandle,
            BuildStatement(
                "CREATE TABLE IF NOT EXISTS ",
                    USERS_TABLE_NAME,
                "(",
                    user.Id.columnName, " SERIAL PRIMARY KEY,",
                    user.Name.columnName, " VARCHAR(", to_string(MAX_NAME_LENGTH).c_str() ,") NOT NULL,",
                    user.Surname.columnName, " VARCHAR(", to_string(MAX_SURNAME_LENGTH).c_str(), ") NOT NULL,",
                    user.Login.columnName, " VARCHAR(", to_string(MAX_LOGIN_LENGTH).c_str(), ") UNIQUE NOT NULL,",
                    user.Email.columnName, " VARCHAR(", to_string(MAX_EMAIL_LENGTH).c_str(), "),",
                    user.Hash[0].columnName, " INT UNSIGNED,",
                    user.Hash[1].columnName, " INT UNSIGNED,",
                    user.Hash[2].columnName, " INT UNSIGNED,",
                    user.Hash[3].columnName, " INT UNSIGNED,",
                    user.Hash[4].columnName, " INT UNSIGNED,",
                    user.RegDate.columnName, " DATE,",
                    user.LastVisit.columnName, " DATETIME,",
                    user.IsLocked.columnName, " BOOL DEFAULT false,",
                    user.IsDeleted.columnName, " BOOL DEFAULT false"
                ")"
            ), SQL_NTS))) {
            cout << "������� ����� ������� �������������.\n";
        }
        else {
            cout << "������ �������� ����� ������� �������������!\n";
            return sqlResult;
        }
    }

    cout << "�������� ������� ���������...";
 
    if (MySQLSuccess(SQLExecDirectA(sqlStmtHandle, BuildStatement("SELECT * FROM ", MSGS_TABLE_NAME, " LIMIT 1"), SQL_NTS))) {
        cout << " ���������.\n";
    } else {
        cout << "\n��������������: ������� ��������� �����������!\n";
        if (MySQLSuccess(sqlResult = SQLExecDirectA(sqlStmtHandle,
            BuildStatement(
                "CREATE TABLE IF NOT EXISTS ",
                    MSGS_TABLE_NAME,
                "(",
                    message.Id.columnName, " SERIAL PRIMARY KEY, ",
                    message.SenderId.columnName, " INT UNSIGNED NOT null references ", USERS_TABLE_NAME, "(", user.Id.columnName, "),",
                    message.ReceiverId.columnName, " INT UNSIGNED NOT null references ", USERS_TABLE_NAME, "(", user.Id.columnName, "),",
                    message.Text.columnName, " VARCHAR(", to_string(MAX_MESSAGE_LENGTH).c_str(), ") NOT NULL,",
                    message.DateTime.columnName, " DATETIME,",
                    message.IsReaded.columnName, " BOOL DEFAULT false,",
                    message.IsDelivered.columnName, " BOOL DEFAULT false"
                ")"
            ), SQL_NTS))) {
            cout << "������� ����� ������� ���������.\n";
        }
        else {
            cout << "������ �������� ����� ������� ���������!\n";
            return sqlResult;
        }
    }

    return sqlResult;
}


// ������������� ���������� � ��������
int Initialize_ODBC() {
    cout << "����������� � SQL-�������...";

    // Allocate environment handle  
    if (MySQLSuccess(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))) {   // �������� ���������� ��� ���� ������
        // Set the ODBC version environment attribute
        if (MySQLSuccess(SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0))) { // ������������� ������� (??)
            // Allocate connection handle  
            if (MySQLSuccess(SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))) {    // �������� ���������� ��� ���������� � ����� ������
                SQLSetConnectAttr(sqlConnHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);  // Set login timeout to 5 seconds  
                
                if (MySQLSuccess(SQLDriverConnectA(sqlConnHandle, GetDesktopWindow(),   // ������������ � ���� ������
                    (SQLCHAR*)"DRIVER={MySQL ODBC 8.0 ANSI Driver};"
                    "SERVER=localhost;"
                    "PORT=3306;"
                    //"DATABASE=chatDB;"
                    "UID=root;"
                    "PWD=root;",
                    SQL_NTS,
                    retconstring,
                    1024,
                    NULL,
                    SQL_DRIVER_COMPLETE))) {
                    
                    if (MySQLSuccess(SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))) { // Allocate statement handle  
                        cout << " ���������.\n";
                        
                        sqlResult = SQLExecDirectA(sqlStmtHandle, BuildStatement("CREATE DATABASE IF NOT EXISTS ", DATABASE_NAME), SQL_NTS);
                        sqlResult = SQLExecDirectA(sqlStmtHandle, BuildStatement("USE ", DATABASE_NAME), SQL_NTS);
                        if (MySQLSuccess(CheckTables()))
                            return 0;   // ������������� ������� �� ��������� �������  
                        else
                            return sqlResult;
                    }
                    else {
                        cout << "������ ��� ��������� ����������� ��� ���������� �������� � ���� ������.\n";
                        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
                        SQLDisconnect(sqlConnHandle);
                    }
                }
                else {
                    cout << "������ ��� ����������� � ���� ������.\n";
                    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
                }
            }
            else {
                cout << "������ ��� ��������� ����������� ��� ���������� � ����� ������.\n";
            }
        }
        else {
            cout << "������ ��� ��������� ��������.\n";
            SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
        }
    }
    else {
        cout << "������ ��� ��������� ����������� ��� ���� ������.\n";
    }
    return SQL_ERROR;
}


// ��������� ���������� � �����������
int Close_ODBC() {
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);

    return 0;
}