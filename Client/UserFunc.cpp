#include "StdAfx.h"

#include "UserFunc.h"
#include "../Server/ServiceFunc.h"


using namespace std;


bool SetName(string& name) {	// ��������� ���� � �������� �����
	cout << "��� (����� " << MIN_NAME_LENGTH << "-" << MAX_NAME_LENGTH << " ��������, ������� �� �����������): ";
	name = get_string_from_console();
	return (check_name(name, "���", MIN_NAME_LENGTH, MAX_NAME_LENGTH) && check_spaces(name, "���"));
}


bool SetSurname(string& surname) {	// ��������� ���� � �������� �������
	cout << "������� (����� " << MIN_SURNAME_LENGTH << "-" << MAX_SURNAME_LENGTH << " ��������, ������� �� �����������): ";
	surname = get_string_from_console();
	return (check_name(surname, "�������", MIN_SURNAME_LENGTH, MAX_SURNAME_LENGTH) && check_spaces(surname, "�������"));
}


bool SetLogin(string& login) {	// ��������� ���� � �������� login
	cout << "������� ��� (login) (����� " << MIN_LOGIN_LENGTH << "-" << MAX_LOGIN_LENGTH << " ��������, ������� �� �����������): ";
	login = get_string_from_console();
	return (check_name(login, "login", MIN_LOGIN_LENGTH, MAX_LOGIN_LENGTH) && check_spaces(login, "login"));	// �������� ����������
}


bool SetEmail(string& email) {	// ��������� ���� � �������� email
	cout << "E-mail (����� " << MIN_EMAIL_LENGTH << "-" << MAX_EMAIL_LENGTH << " ��������, ������� �� �����������): ";
	email = get_string_from_console();
	return (check_name(email, "e-mail", MIN_EMAIL_LENGTH, MAX_EMAIL_LENGTH) && check_spaces(email, "e-mail"));	// �������� ����������
}


bool SetPassword(string& password) {	// ��������� ���� � �������� ������
	cout << "������: ";
	password = get_value_from_console();
	if (!check_name(password, "������", MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH) || !check_spaces(password, "������")) {
		password = "";
		return false;
	}
	cout << "��������� ������: ";
	string password2 = get_value_from_console();
	if (password != password2) {
		cout << "������ �� ���������!\n";
		password = "";
		return false;
	}

	return true;
}


bool NewUser(string& name, string& surname, string& login, string& email, string& password) {	// �������� �������� ��� ����������� ������ ������������
	cout << "��� ����������� ������� ��������� ������ ������������ (������� �� �����������):\n";
	return (SetName(name) && SetSurname(surname) && SetLogin(login) && SetEmail(email) && SetPassword(password));
}


bool SignIn(string& login, string& password) {	// �������� �������� ��� ����� ������������ � ���
	cout << "������� �������� ��� (login): ";
	cin >> login;
	if (!check_empty_name(login, "���"))
		return false;

	cout << "������� ������: ";
	cin >> password;

	return true;
}


bool ChooseUser(string& receiver) {	// �������� ���������� ��� �������� ���������
	cout << "������� �������� ��� (login) ����������: ";
	cin >> receiver;
	return (check_empty_name(receiver, "���"));
}


bool AddMessage(string& text) {	// ��������� ���������
	cout << "������� ����� ���������: ";
	text = get_string_from_console();
	return (check_name(text, "���������", 1, MAX_MESSAGE_LENGTH));
}

bool SetRegData(string& changed_data, const uchar regDataType) {	// ��������� ������ � ������� ����� ��������������� ������
	bool bResult = false;

	switch (regDataType) {
		case SLCMD_CHANGE_NAME:	// ��������� ������� name - �������� ��� ������������
			bResult = SetName(changed_data);
			break;
		case SLCMD_CHANGE_SURNAME:	// ��������� ������� surname - �������� ������� ������������
			bResult = SetSurname(changed_data);
			break;
		case SLCMD_CHANGE_LOGIN:	// ��������� ������� log - �������� ������� ��� (login) ������������
			bResult = SetLogin(changed_data);
			break;
		case SLCMD_CHANGE_EMAIL:	// ��������� ������� email - �������� e-mail ������������
			bResult = SetEmail(changed_data);
			break;
		case SLCMD_CHANGE_PASSWORD:	// ��������� ������� pwd - �������� ������ ������������
			bResult = (SetPassword(changed_data));
			break;
	}

	return bResult;
}