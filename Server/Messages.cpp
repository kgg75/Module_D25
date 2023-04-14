// ������� ��� �������� � ���� ������ �������� �� ����������
#include "StdAfx.h"

#include "Messages.h"
#include "Users.h"
#include "DBFunctions.h"
#include "ServiceFunc.h"


using namespace std;

// ���������� ��� ������������ �������� ������, ��������� � ����������, ��� �� �����, �������� � �.�.
Message message;


/**************************************
  AddMessageToDB ��������� ��������� � ���� ������
  -------------------------------------
  const uint senderId		- ������������� �����������
  const uint receiverId		- ������������� ����������
  const string& text		- ������ �� ����� ���������
  -------------------------------------
  ���������� ������������� ���������, ����������� ����� ������, ��� ��� ������
  *************************************/

int AddMessageToDB(const uint senderId, const uint receiverId, const string& text) {
	SQLRETURN sqlResult;

	sqlResult = SQLCloseCursor(sqlStmtHandle);
	sqlResult = SQLExecDirectA(sqlStmtHandle, BuildStatement(
		"INSERT INTO ",
			MSGS_TABLE_NAME,
			"(",
				message.SenderId.columnName, ",",
				message.ReceiverId.columnName, ",",
				message.Text.columnName, ",",
				message.DateTime.columnName,
			") "
		"VALUES (",
			to_string(senderId).c_str(), ",",
			to_string(receiverId).c_str(), ",'",
			text.c_str(), 
			"', CURRENT_TIMESTAMP"
		")"
	), SQL_NTS);

	if (sqlResult == SQL_SUCCESS) {	// �������� id ���������
		sqlResult = SQLCloseCursor(sqlStmtHandle);
		sqlResult = SQLExecDirectA(sqlStmtHandle, BuildStatement(
			"SELECT ",
				"MAX(", message.Id.columnName, ")"
			" FROM ",
				MSGS_TABLE_NAME
		), SQL_NTS);

		SQLLEN		row_count;
		SQLSMALLINT	col_count;

		SQLNumResultCols(sqlStmtHandle, &col_count);
		SQLRowCount(sqlStmtHandle, &row_count);

		if (sqlResult == SQL_SUCCESS && col_count == 1 && row_count == 1) {	// 1 - ����� �������� � ����������� ���� �������
			sqlResult = SQLBindCol(sqlStmtHandle, 1, user.Id.targetType, &user.Id.value, sizeof(user.Id.value), nullptr);
			if (sqlResult == SQL_SUCCESS && SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
				return user.Id.targetType;	// ���������� id ���������
		}
	}

	return ERR_REQUEST_ERROR;
}


/**************************************
  SetReadedState ������������� ��������� ����� "��������" ��� ���� ��� ������ ��������� ���������� ������������
  -------------------------------------
  const uint senderId		- ������������� �����������
  const uint messageId		- ������������� ���������; �� ��������� 0
  const bool MarkAsReaded	- ��������� ����� "��������" ��� ���������; �� ��������� false
  -------------------------------------
  ���� ������������� ��������� �� ������ (�� ��������� ��������������� �������� 0),
  �� ������� ������������� ���� MarkAsReaded ��� ���� ��������� ������������.
  -------------------------------------
  ���������� MSG_SUCCESS_RESULT ��� ��� ������
  *************************************/

int SetReadedState(const uint receiverId, const uint messageId, const bool MarkAsReaded) {
	SQLRETURN sqlResult;

	sqlResult = SQLExecDirectA(sqlStmtHandle,
		BuildStatement(
		"UPDATE ",
			MSGS_TABLE_NAME,
		" SET ",
			message.IsReaded.columnName, (MarkAsReaded ? "='1'" : "='0'"),
		" WHERE ",
			(messageId == 0 ? message.ReceiverId.columnName : message.Id.columnName),
			"=",
			to_string((messageId == 0 ? receiverId : messageId)).c_str()
		), SQL_NTS);
	
	return (sqlResult == SQL_SUCCESS) ? MSG_SUCCESS_RESULT : ERR_REQUEST_ERROR;
}


/**************************************
  GetMessagesCount ������������ ����� ��������� ��� ���������� ����������
  -------------------------------------
  const uint receiverId			- ������������� ����������
  const bool showUnReadedOnly	- ����, ��������������� ������ ���������: ������ ������������� - true (�� ���������) ��� ��� - false
  -------------------------------------
  ���������� ����� ��������� ��� ��� ������
  *************************************/

uint GetMessagesCount(const uint receiverId, const bool showUnReadedOnly) {
	uint count = 0;
	SQLRETURN sqlResult;

	sqlResult = SQLCloseCursor(sqlStmtHandle);
	sqlResult = SQLExecDirectA(sqlStmtHandle, BuildStatement(
		"SELECT ",
			"COUNT(", message.ReceiverId.columnName, ")"
		" FROM ",
			MSGS_TABLE_NAME,
		" WHERE ",
			message.ReceiverId.columnName, "=", to_string(receiverId).c_str(),
		" AND ",
			message.IsReaded.columnName, (showUnReadedOnly ? "=0" : "<=1")
	), SQL_NTS);

	SQLLEN		row_count;
	SQLSMALLINT	col_count;

	SQLNumResultCols(sqlStmtHandle, &col_count);
	SQLRowCount(sqlStmtHandle, &row_count);

	if (sqlResult == SQL_SUCCESS && col_count == 1 && row_count == 1) {	// 1 - ����� �������� � ����������� ���� �������
		sqlResult = SQLBindCol(sqlStmtHandle, 1, SQL_INTEGER, &count, sizeof(count), nullptr);
		if (sqlResult == SQL_SUCCESS && SQLFetch(sqlStmtHandle) == SQL_SUCCESS)
			return count;
	}

	return ERR_REQUEST_ERROR;
}


/**************************************
  PrepareListByOneM �������������� ���������� ������� ListByOneM
  -------------------------------------
  const uint receiverId			- ������������� ����������
  const bool showUnReadedOnly	- ����, ��������������� ������ ���������: ������ ������������� - true (�� ���������) ��� ��� - false
  -------------------------------------
  ���������� ����� ��������� ��� ��� ������
  *************************************/

int PrepareListByOneM(const uint receiverId, const bool showUnReadedOnly) {
	SQLRETURN sqlResult;

	sqlResult = SQLCloseCursor(sqlStmtHandle);
	sqlResult = SQLExecDirectA(sqlStmtHandle, BuildStatement(
		"SELECT ",
			MSGS_TABLE_NAME, ".", message.Id.columnName, ",",
			user.Login.columnName, ",",
			message.Text.columnName, ",",
			message.DateTime.columnName,
		" FROM ",
			MSGS_TABLE_NAME, " JOIN ", USERS_TABLE_NAME,
		" ON ",
			USERS_TABLE_NAME, ".", user.Id.columnName, "=", MSGS_TABLE_NAME, ".", message.SenderId.columnName,
		" WHERE ",
			message.ReceiverId.columnName, "=", to_string(receiverId).c_str(),
		" AND ",
			message.IsReaded.columnName, (showUnReadedOnly ? "=0" : "<=1"),
		" ORDER BY ",
			message.DateTime.columnName
	), SQL_NTS);

	if (sqlResult == SQL_SUCCESS) {
		SQLLEN		row_count;
		SQLSMALLINT	col_count;

		SQLNumResultCols(sqlStmtHandle, &col_count);
		SQLRowCount(sqlStmtHandle, &row_count);

		if (sqlResult == SQL_SUCCESS && col_count == 4 && row_count > 0) {	// 4 - ����� �������� � ����������� ���� �������
			sqlResult = SQLBindCol(sqlStmtHandle, 1, message.Id.targetType, &message.Id.value, sizeof(message.Id.value), nullptr);
			sqlResult = SQLBindCol(sqlStmtHandle, 2, user.Login.targetType, user.Login.value, MAX_LOGIN_LENGTH, nullptr);
			sqlResult = SQLBindCol(sqlStmtHandle, 3, message.Text.targetType, message.Text.value, MAX_MESSAGE_LENGTH, nullptr);
			sqlResult = SQLBindCol(sqlStmtHandle, 4, message.DateTime.targetType, message.DateTime.value, sizeof(SQL_DATETIME_FORMAT), nullptr);
			return row_count;
		}
	}

	return ERR_REQUEST_ERROR;
}


/**************************************
  ListByOneM ���������� ��������� ������������, ������������ � �������������� ������ PrepareListByOneM(),
  � ������� �� ����� � ������� �������� � ���.
  ������ ���������� ������ ����� PrepareListByOneM().
  -------------------------------------
  const uint user_index		- ���������� ����� ������������ ������
  string& output			- ������ ��� ������ ����������
  -------------------------------------
  ���������� ����� ������ output ��� ERR_NO_RESULT
  *************************************/

int ListByOneM(const uint user_index, string& output) {
	while (SQLFetch(sqlStmtHandle) != SQL_NO_DATA) {
		output = /*"   " + to_string(user_index) +
			") "*/"  #" + to_string(message.Id.value) +
			" [" + (string)message.DateTime.value +
			"] �� '" + (string)user.Login.value +
			"': " + (string)message.Text.value + '\n';
		return output.length();
	}

	return ERR_NO_RESULT;
}
