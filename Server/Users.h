#pragma once
#include "SHA1.h"


// ��������� ������ ��������� � ������������� ����������, � ����� �������������� �� ��������� ��� ���� ������
struct User {
	Var_with_AddParams <uint>									Id			{ 0,		"id",			SQL_INTEGER };
	Var_with_AddParams <char[MAX_NAME_LENGTH + 1]>				Name		{ "",		"first_name",	SQL_CHAR };	// +1 ������ �� 0 � �����
	Var_with_AddParams <char[MAX_SURNAME_LENGTH + 1]>			Surname		{ "",		"surname",		SQL_CHAR };	// +1 ������ �� 0 � �����
	Var_with_AddParams <char[MAX_LOGIN_LENGTH + 1]>				Login		{ "",		"login",		SQL_CHAR };	// +1 ������ �� 0 � �����
	Var_with_AddParams <char[MAX_EMAIL_LENGTH + 1]>				Email		{ "",		"email",		SQL_CHAR };	// +1 ������ �� 0 � �����
	Var_with_AddParams <uint>									Hash[SHA1_HASH_LENGTH_UINTS]{
																			{ 0,		"hash1",		SQL_INTEGER },
																			{ 0,		"hash2",		SQL_INTEGER },
																			{ 0,		"hash3",		SQL_INTEGER },
																			{ 0,		"hash4",		SQL_INTEGER },
																			{ 0,		"hash5",		SQL_INTEGER } };
	Var_with_AddParams <char[sizeof(SQL_DATETIME_FORMAT) + 1]>	RegDate		{ "",		"reg_date",		SQL_CHAR };	// +1 ������ �� 0 � �����
	Var_with_AddParams <char[sizeof(SQL_DATETIME_FORMAT) + 1]>	LastVisit	{ "",		"last_visit",	SQL_CHAR };	// +1 ������ �� 0 � �����
	Var_with_AddParams <bool>									IsLocked	{ false,	"is_locked",	SQL_BIT };
	Var_with_AddParams <bool>									IsDeleted	{ false,	"is_deleted",	SQL_BIT };
};


extern User user;


int GetUserId(const std::string& login);	// ��������� ������������ ��������� login ������������; ���������� id ��� ��� ������
int GetLogin(const uint id, std::string& login);	// �������� login ��� ���������� �� id ������������; ���������� MSG_SUCCESS_RESULT ��� ��� ������
int CheckLogin(const std::string& login);	// �������� login �� ������������ � �������������
int CheckHash(const uint id, const SHA1PwdArray& pass_sha1_hash);	// ��������� ���� ������������ � ���; ���������� ��� id ��� ��� ������
int NewUser(const std::string& name, const std::string& surname, const std::string& login, const std::string& email, SHA1PwdArray& pass_sha1_hash);	// ������������ ������ ������������; ���������� ��� id ��� ��� ������
int ChangeRegData(const uint id, const char* changed_data, const uchar regDataType);
int SetPassword(const uint id, SHA1PwdArray& pass_sha1_hash);	// �������� ��� ������ ������������
int SetLastVisit(const uint id);	// ��������� ���� � ����� ���������� ���������
int SetIsDeleted(const uint id);	// �������� ������������ ��� ���������
int UserInfo(const uint id, std::string& output);	// ���������� ������ ���������� ������������, ���������� ����� ������ output
int PrepareListByOneU();	// ������� ������� �������� �� ���������� ������������; ���������� ����� ������ ��� ��� ������
int ListByOneU(const uint user_index, std::string& output);	// ������� ������� �������� �� ���������� ������������; ��������� ������ output � ���������� � ����� ��� ��� ������
