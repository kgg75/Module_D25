#pragma once


constexpr auto SQL_RESULT_LEN = 240;
constexpr auto SQL_RETURN_CODE_LEN = 1024;


extern SQLHSTMT	sqlStmtHandle;  // ���������� ��� ���������� �������� � ���� ������


// ���������� ��� ������� SQLGetDiagRecA - ���������� ������� �������� ���������� ����� ��������������� ������,
// ���������� �������� �� ������, �������������� � ���������.
#if _DEBUG 
	extern SQLCHAR		SQLState;
	extern SQLINTEGER	NativeError;
	extern SQLCHAR		ErrorMessageText[];
	extern SQLSMALLINT	ErrorTextLength;
#endif


int MySQLSuccess(SQLRETURN rc);
int Initialize_ODBC();
int Close_ODBC();