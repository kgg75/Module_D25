#pragma once
#include "Constants.h"


uchar get_command_code(const CommandSpace* commandSpace, std::string& command);	// ��������� ������ ������� � ���������� �������������� ��� ��������
uchar get_command(const CommandSpace* commandSpace, const std::string console_prompt = "������� ������� >> ");	// ��������� ��������� ������� � �������, ���������� �������������� ��� ��������
void lowercase(std::string& str);	// ����������� ������ � ������ �������

// ���������� ����� ������, ������� ������ '0' � �����
inline uint full_length(const std::string& str) {
	return str.length() + 1;
}

// �������� �� ������ dest ������ source, ������� ������ '0' � �����
// ���������� ������ ����� ������ (������� ������ '0' � �����)
inline uint strcopy(char* dest, const std::string& source) {
	uint len = full_length(source);
	memcpy(dest, source.c_str(), len);
	return len;
}

std::string lowercase_s(const std::string& str);	// ����������� ������ � ������ �������
bool check_empty_name(const std::string& str, const std::string& subject);	// ��������� ������ �� �������
bool check_spaces(const std::string& str, const std::string& subject);	// ��������� ������ �� �������
bool check_name(const std::string& str, const std::string& subject, const size_t minLength, const size_t maxLength);	// ��������� ������ �� �������, �����������/������������ �����
std::string get_value_from_console();	// �������� �������� � �������
std::string get_string_from_console();	// �������� ������ � ������� � ������� � �� ��������� � �������� ��������
std::string GetTime();	// ���������� ������� �����
bool QuestionYN(const std::string& message);	// ����� ������ � ����� ���������� ������ y/n � ���������� �����
std::string BoolToStrR(bool value);
std::string BoolToStrE(bool value);
void PrintErrorMessage(const char error_code);

// "����������" ����� const char[] ��� �������� � SQL-������
template <typename ... Args> SQLCHAR* BuildStatement(const Args* ...args) {
	static char buffer[SQL_BUFFER_LENGTH];
	uint	len, index = 0;
	(
		(
			len = strlen(args),		// ��������� ����� ���������� ��������� args
			memcpy(&buffer[index], args, len),   // �������� � ����� ������ ������ �� ������ ���������� args
			index += len	// ������������� ������
		),
		...   // ���������� ������ ���������� args  - C++17
	);

	buffer[index] = 0;	// ������������� 0 � ����� ������

	return (SQLCHAR*)buffer;
}