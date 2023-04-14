#pragma once
#include "../Server/Constants.h"


bool SetName(std::string& name);	// ��������� ���� � �������� �����
bool SetSurname(std::string& surname);	// ��������� ���� � �������� �������
bool SetLogin(std::string& login);	// ��������� ���� � �������� login
bool SetEmail(std::string& login);	// ��������� ���� � �������� email
bool SetPassword(std::string& password);	// ��������� ���� � �������� ������
bool NewUser(std::string& name, std::string& surname, std::string& login, std::string& email, std::string& password);	// �������� �������� ��� ����������� ������ ������������
bool SignIn(std::string& login, std::string& password);	// �������� �������� ��� ����� ������������ � ���
bool ChooseUser(std::string& receiver);	// �������� ���������� ��� �������� ���������
bool AddMessage(std::string& text);	// ��������� ���������
bool SetRegData(std::string& changed_data, const uchar regDataType);	// ��������� ������ ����� ��������������� ������