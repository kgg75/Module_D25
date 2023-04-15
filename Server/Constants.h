#pragma once


typedef unsigned int uint;
typedef unsigned char uchar;


// ��������� ��������� ������ ������� - ���������
struct CommandSpace {
	const uchar code;		// ��� �������
	const char* command;	// ����� �������
	const char* command2;	// ����������� ������� ������ ������� ��� ������� - ���������������� � �������!
	const char* help;		// ����� ������� �� �������
};


// ��������� ��������� ������ ��� ������ - ���������
struct ErrorSpace {
	const char code;
	const char* text;
};


// ��������� ��������� ���������� � �������� ��������� � ��� �������� ���� ������
template <typename T> struct Var_with_AddParams {
	T value;						// �������� ����������
	const char* columnName;			// ��� ���������������� ������� � ���� ������
	const SQLSMALLINT targetType;	// ��� ������
};


// ���� ������
const uchar
	// ���������
		//SCMD_SAVE =				1,
		SCMD_CON =				2,
		SCMD_USERS =			3,
		SCMD_HELP =				4,
		SCMD_EXIT =				5,
	// ����������
		GCMD_REG =				20,
		GCMD_LOGIN =			21,
		GCMD_HELP =				22,
		GCMD_EXIT =				23,
	// ���������
		LCMD_SEND =				50,
		LCMD_READ =				51,
		LCMD_READ_ALL =			52,
		LCMD_READ_UNREADED =	53,
		LCMD_USER_INFO =		54,
		LCMD_USERS_LIST =		55,
		LCMD_USER_DELETE =		56,
		LCMD_USER_CHANGE =		57,
		LCMD_HELP =				58,
		LCMD_LOGOUT =			59,
		LCMD_IS_USER_EXIST =	70,
		LCMD_MESSAGES_COUNT =	71,
		LCMD_UNREADED_COUNT =	72,
		LCMD_RECEIVE =			80,
	// ������������
		SLCMD_CHANGE_NAME =		90,
		SLCMD_CHANGE_SURNAME =	91,
		SLCMD_CHANGE_LOGIN =	92,
		SLCMD_CHANGE_EMAIL =	93,
		SLCMD_CHANGE_PASSWORD =	94,
		SLCMD_HELP =			95,
		SLCMD_QUIT =			96;


// ������������ ��������� ������ � �����������
const CommandSpace SERVER_COMMANDS[]{
	//{SCMD_SAVE, "save", "s", "��������� ������ ������������� � ���������"},
	{SCMD_CON,				"con",		"c",	"������� ������ �����������"},
	{SCMD_USERS,			"users",	"u",	"������� ������ �������������"},
	{SCMD_HELP,				"help",		"?",	"������� ������� �� ��������"},
	{SCMD_EXIT,				"exit",		"e",	"������� ������"},
	{0, "", "", ""}
};


// ������������ ���������� ������ � �����������
const CommandSpace GLOBAL_COMMANDS[] {
	{GCMD_REG,				"reg",		"r",	"������������������"},
	{GCMD_LOGIN,			"login",	"l+",	"����� � ���"},
	{GCMD_HELP,				"help",		"?",	"������� ������� �� ��������"},
	{GCMD_EXIT,				"exit",		"e",	"������� ���"},
	{0, "", "", ""}
};


// ������������ ��������� ������ � �����������
const CommandSpace LOCAL_COMMANDS[] {
	{LCMD_SEND,				"send",		"s",	"��������� ���������"},
	{LCMD_READ,				"read",		"r",	"��������� ���������"},
	{LCMD_USER_INFO,		"info",		"i",	"������� ���������� � ������������"},
	{LCMD_USERS_LIST,		"list",		"l",	"������� ������ ���� �������������"},
	{LCMD_USER_DELETE,		"delete",	"del",	"������� ������� ������ ������������"},
	{LCMD_USER_CHANGE,		"change",	"ch",	"�������� ������� ������ ������������"},
	{LCMD_HELP,				"help",		"?",	"������� ������� �� ��������"},
	{LCMD_LOGOUT,			"logout",	"l-",	"����� �� ����"},
	{0, "", "", ""}
};


// ��������������� ��������� ������ ��� ��������� ��������������� ������ � �����������
const CommandSpace SUBLOCAL_COMMANDS[] {
	{SLCMD_CHANGE_NAME,		"name",		"n",	"�������� ��� ������������"},
	{SLCMD_CHANGE_SURNAME,	"surname",	"sn",	"�������� ������� ������������"},
	{SLCMD_CHANGE_LOGIN,	"login",	"l",	"�������� ������� ��� (login) ������������"},
	{SLCMD_CHANGE_EMAIL,	"email",	"em",	"�������� e-mail ������������"},
	{SLCMD_CHANGE_PASSWORD,	"pwd",		"p",	"�������� ������ ������������"},
	{SLCMD_HELP,			"help",		"?",	"������� ������� �� ��������"},
	{SLCMD_QUIT,			"quit",		"q",	"����� �� ����� ����"},
	{0, "", "", ""}
};


// ���� ������ � ��������������� ���������
const char
	MSG_SUCCESS_RESULT =		1,
	MSG_STOP_EXCHANGE =			0,
	ERR_NO_RESULT =				0,
	ERR_USER_NOT_FOUND =		-1,
	ERR_WRONG_PASSWORD =		-2,
	ERR_USER_ALREADY_LOGGED =	-3,
	ERR_LOGIN_ALREADY_EXIST =	-4,
	ERR_TRY_USE_SERVICE_NAME =	-5,
	ERR_WRONG_USER_ID =			-6,
	ERR_REQUEST_ERROR =			-50,	// ��� ������������� ������ �������
	ERR_SERVER_ERROR =			-51;	// ��� �������� �������


const ErrorSpace ERROR_SPACE[]{
	{ERR_USER_NOT_FOUND,		"������������ � ����� login �� ����������!"},
	{ERR_WRONG_PASSWORD,		"�������� ������!"},
	{ERR_USER_ALREADY_LOGGED,	"������������ � ����� login ��� �������� ���� � ��� � ������� ������!"},
	{ERR_LOGIN_ALREADY_EXIST,	"������������ � ����� login ��� ���������������!"},
	{ERR_TRY_USE_SERVICE_NAME,	"���������������� ��� ����� ������ ������!"},
	{ERR_REQUEST_ERROR,			"������ ���������� ������� � ���� ������."},
	{ERR_SERVER_ERROR,			"������ �������."},
	{0, ""}
};


//����������� ��������
const int
	PORT_NUMBER =			7777,	// ����� �����
	DELAY_MS =				400,	// �������� ��� ������� � ��
	DELAY_MCS =				DELAY_MS * 1'000;	// �������� ��� ������� � ���


const size_t
	MIN_NAME_LENGTH =		2,		// ����������� ����� ����� ������������
	MAX_NAME_LENGTH =		16,		// ������������ ����� ����� ������������
	MIN_SURNAME_LENGTH =	2,		// ����������� ����� ����� ������������
	MAX_SURNAME_LENGTH =	32,		// ������������ ����� ����� ������������
	MIN_LOGIN_LENGTH =		2,		// ����������� ����� login
	MAX_LOGIN_LENGTH =		32,		// ������������ ����� login
	MIN_EMAIL_LENGTH =		5,		// ����������� ����� email
	MAX_EMAIL_LENGTH =		32,		// ������������ ����� email
	MIN_PASSWORD_LENGTH =	3,		// ����������� ����� ������
	MAX_PASSWORD_LENGTH =	32,		// ������������ ����� ������
	BUFFER_LENGTH =			1024,	// ������ ������ ��� ������
	SQL_BUFFER_LENGTH =		1024,	// ������ ������ ��� SQL-�������
	MAX_MESSAGE_LENGTH =	512;	// ������������ ������ ���������


const char
	SERVER_ADDRESS[]		{ "127.0.0.1" },
	CLIENT_STOP_MESSAGE[]	{ "stop" },
	SERVICE_NAME[]			{ "��� ������������" },	// ��� ���������� ������������ � �������� 0 (�� ������������)
	SERVICE_LOGIN[]			{ "all" },	// login ���������� ������������ � �������� 0 (�� ������������)
	SEPARATOR[]				{ "-------------------------------------------------------------------------\n" },
	MSG_ERR_CMD[]			{ "������� �� ����������, ��������� ����.\n" },
	DATE_TIME_FORMAT[]		{ "[%Y-%m-%d %H:%M:%S]" },
	SQL_DATETIME_FORMAT[]	{ "[yyyy-MM-dd hh:mm:ss]" },
	USERS_TABLE_NAME[]		{ "users" },	// ��� ������� ��� �������� ������ �������������
	MSGS_TABLE_NAME[]		{ "messages" },	// ��� ������� ��� �������� ���������
	DATABASE_NAME[]			{ "chatDB" };	// ��� ���� ������
