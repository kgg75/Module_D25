#include "StdAfx.h"

#include "Client.h"
#include "UserFunc.h"
#include "../Server/Constants.h"
#include "../Server/ServiceFunc.h"
#include "../Server/SHA1.h"


using namespace std;


char			request_msg[BUFFER_LENGTH],
				response_msg[BUFFER_LENGTH];

uint			request_msg_length,
				response_msg_length;

SOCKET			server_descriptor;

fd_set			descriptors_set {0};

const timeval	timeout = {0, 0};

string			console_prompt;

bool			loop = true;	// ���� �����������/��������� ChatClient()


// ���������� ������� � �������
int client_start() {
	int iResult;

	WIN(
		WSADATA wsaData = {0};
		WSAStartup(MAKEWORD(2, 2), &wsaData);
	)

	server_descriptor = socket(AF_INET, SOCK_STREAM, 0);	// �������� �����
	if (server_descriptor WIN(== INVALID_SOCKET)NIX(< 0)) {
		cout << "����� �� ����� ���� ������!\n";
		WIN(cout << "��� ������: " << WSAGetLastError() << '\n';)
		return -1;
	}

	struct sockaddr_in server_address;
	inet_pton(AF_INET, SERVER_ADDRESS, &server_address.sin_addr.s_addr);    // ��������� ����� �������
	server_address.sin_port = htons(PORT_NUMBER);    // ������� ����� �����
	server_address.sin_family = AF_INET;    // ���������� IPv4

	// ��������� ���������� � ��������
	iResult = connect(server_descriptor, (struct sockaddr*)&server_address, sizeof(server_address));
	if (iResult WIN(== SOCKET_ERROR)NIX(!= 0)) {
		cout << "���������� � �������� �� �����������!\n";
		WIN(cout << "��� ������: " << WSAGetLastError() << '\n';)
		return -1;
	}
	return 0;
}


// ��������� ������� �� �������
void client_stop() {	// ��������� �����, ��������� ����������
	WIN(send(server_descriptor, CLIENT_STOP_MESSAGE, sizeof(CLIENT_STOP_MESSAGE), 0))	// ����������� ������ � ���� ����������
	NIX(write(server_descriptor, CLIENT_STOP_MESSAGE, sizeof(CLIENT_STOP_MESSAGE)));

	WIN(closesocket)NIX(close)(server_descriptor);

	WIN(WSACleanup());
}


// ������� ������� � ������� �����������
void ConsoleWaitMessage() {
	cin.clear();
	//cin.ignore(numeric_limits<streamsize>::max(), '\n');	// ������� ������ �������
	cout << console_prompt;
}


// �������� ������� �� ��������
void print_help(const CommandSpace* commandSpace) {
	for (int i = 0; commandSpace[i].command[0] != 0; i++)
		cout << "\t" << commandSpace[i].command << "\t- " << commandSpace[i].help << '\n';
}


// �������� ��������� ���������
void AtStart() {	
	cout.clear();
	cout << SEPARATOR;
	cout << "* * *  ���-������ ������ 2.0  * * *\n";
	cout << SEPARATOR;
	cout << "��� ������� " << GetTime() << '\n';
	cout << SEPARATOR;
	cout << "�������� ��������:\n";
	print_help(GLOBAL_COMMANDS);
	cout << SEPARATOR;
}


// �������� ����� �������� ����������� (��������/����)
int MessagesExchange(const uchar command_code) {
	request_msg[0] = command_code;	// ��������� ��� ������� � ������ [0];

	int iResult =	WIN(send(server_descriptor, request_msg, request_msg_length, 0))
					NIX(write(server_descriptor, request_msg, request_msg_length));

	if (iResult WIN(== SOCKET_ERROR)NIX(!= 0)) {	// ���� �������� >= 0  ����, ������ ��������� ������ �������
		cout << "������ �������� ������ �� ������!\n";
		WIN(cout << "��� ������: " << WSAGetLastError() << '\n';)
	}

	iResult =	WIN(recv(server_descriptor, response_msg, BUFFER_LENGTH, 0))	// �������� �����
				NIX(read(server_descriptor, response_msg, BUFFER_LENGTH));

	if (iResult WIN(== SOCKET_ERROR)NIX(!= 0)) {	// ���� �������� >= 0  ����, ������ ���� ������ �������
		cout << "������ ��������� ������ � �������!\n";
		WIN(cout << "��� ������: " << WSAGetLastError() << '\n';)
	}

	return iResult;
}


// ��������� ������ ����� ���������
uint RequestMessagesQty(const string& receiver, const uchar flag_UnReadedOrAll) {	
	strcopy(&request_msg[1], receiver);	// �������� login � ����� ��� ��������
	request_msg_length = 1 + full_length(receiver);
	MessagesExchange(flag_UnReadedOrAll);	// �������� ���������� ���������

	uint unReadedMsgs;
	memcpy(&unReadedMsgs, &response_msg[1], sizeof(unReadedMsgs));
	return unReadedMsgs;
}


// ��������� ����������� ������ � ������ ������ ���������
void PrintMessages(const uchar command_code, const string& login) {
	strcopy(&request_msg[1], login);	// �������� login � ����� ��� ��������
	request_msg_length = 1 + full_length(login);

	do {
		MessagesExchange(command_code);	// �������������� �������� � ������������ ����������� �������� ���������
		if (response_msg[0] == MSG_SUCCESS_RESULT)
			cout << &response_msg[1];
	} while (response_msg[0] == MSG_SUCCESS_RESULT);

	if (response_msg[0] < 0)	// < 0 - ��������� ��� ������
		PrintErrorMessage(response_msg[0]);
}


void RequestHandler() {
	string sender, text;
	int iResult, response_msg_length = 1;
	//uint count, index = 1, receiverId;
	uchar command_code = request_msg[0];

	WIN(memset(response_msg, 0, BUFFER_LENGTH))NIX(bzero(response_msg, BUFFER_LENGTH));
	response_msg[0] = MSG_SUCCESS_RESULT;	// ������������� �������� ������ (1-� ����) �� ���������

	switch (command_code) {
		case LCMD_RECEIVE:
			sender = &request_msg[1];
			text = &request_msg[1 + sender.length() + 1];
			cout << '\n' << GetTime() << " �������� ��������� �� '" << sender << "': " << text << '\n';
			ConsoleWaitMessage();
			
			response_msg[0] = MSG_SUCCESS_RESULT;
			response_msg_length = 1;
			iResult = WIN(send(server_descriptor, response_msg, response_msg_length, 0))	// TODO!! ������� � �������
				NIX(write(server_descriptor, response_msg, response_msg_length));

			if (iResult WIN(== SOCKET_ERROR)NIX(!= 0)) {	// ���� �������� >= 0  ����, ������ ��������� ������ �������
				cout << "������ �������� ������ �� ������!\n";
				WIN(cout << "��� ������: " << WSAGetLastError() << '\n';)
			}
			;
			break;
		default:
			;
			break;
	}
}


// ���������� ���������� ������ ������� ����
int ConsoleCommandsHandler() {	
	string login, new_login, name, surname, email, password, receiver, text, any_text;
	SHA1PwdArray pass_sha1_hash;	// ��� ������
	uint index;

	AtStart();

	while (loop) {
		WIN(memset(request_msg, 0, BUFFER_LENGTH))NIX(bzero(request_msg, BUFFER_LENGTH));

		switch (auto g_command_code = get_command(GLOBAL_COMMANDS)) {	// �������� ���������� ������� � �������
			case GCMD_REG:	// ������� reg - ������������������
				if (NewUser(name, surname, login, email, password)) {
					index = 1;
					index += strcopy(&request_msg[index], name);	// �������� name
					index += strcopy(&request_msg[index], surname);	// �������� surname
					index += strcopy(&request_msg[index], login);	// �������� login
					index += strcopy(&request_msg[index], email);	// �������� email

					CalculateSHA1(pass_sha1_hash, password.c_str());

					memcpy(&request_msg[index], &pass_sha1_hash, SHA1_HASH_LENGTH_BYTES);	// �������� ���
					request_msg_length = index + SHA1_HASH_LENGTH_BYTES;
					MessagesExchange(g_command_code);

					if (response_msg[0] == MSG_SUCCESS_RESULT)	// ������������ ������� �����������������
						cout << "�� ������� ����������������!\n";
					else
						PrintErrorMessage(response_msg[0]);
				}
				break;
			case GCMD_LOGIN:	// ������� login - ����
				if (SignIn(login, password)) {
					index = 1;
					index += strcopy(&request_msg[index], login);	// �������� login

					CalculateSHA1(pass_sha1_hash, password.c_str());

					memcpy(&request_msg[index], &pass_sha1_hash, SHA1_HASH_LENGTH_BYTES);	// �������� ���
					request_msg_length = index + SHA1_HASH_LENGTH_BYTES;
					MessagesExchange(g_command_code);

					if (response_msg[0] == MSG_SUCCESS_RESULT) {	// ������������ ������� �������������
						cout << "������������ '" << login << "' ����� � ���.\n";

						uint unReadedMsgs = RequestMessagesQty(login, LCMD_UNREADED_COUNT);
						if (unReadedMsgs > 0) {
							cout << "���� ������������� ��������� (" << unReadedMsgs << "). ";
							if (QuestionYN("��������?"))	// ����� ������������� ���������
								PrintMessages(LCMD_READ_UNREADED, login);
						}
						
						console_prompt = login + ", ������� ������� >> ";
						while (!login.empty()) {	// ���� ������������ �� ����� �� ����
							WIN(memset(request_msg, 0, BUFFER_LENGTH))NIX(bzero(request_msg, BUFFER_LENGTH));
							request_msg_length = 1;	// ������������� ����������� ����� ���������

							// ������ ���������� ������������ ������
							switch (auto l_command_code = get_command(LOCAL_COMMANDS, console_prompt)) {	// �������� ��������� ������� � �������
								case LCMD_SEND:	// ��������� ������� send - ��������� ���������
									if (!ChooseUser(receiver))	// ����� ����������
										break;
										
									request_msg_length += strcopy(&request_msg[1], receiver);	// �������� receiver
									MessagesExchange(LCMD_IS_USER_EXIST);	// ��������� ������������� ����������

									if (response_msg[0] == MSG_SUCCESS_RESULT) {
										if (receiver != login) {
											if (AddMessage(text)) {
												index = 1;
												index += strcopy(&request_msg[index], receiver);	// �������� ��� ����������
												request_msg_length = index + strcopy(&request_msg[index], text);	// �������� text, ������������� ����� ���������
												
												MessagesExchange(l_command_code);

												if (response_msg[0] != 0)
													cout << "��������� ��� '" << receiver << "' ����������.\n";
											}
										} else
											cout << "������ ���������� ��������� ������ ����.\n";
									}
									else
										PrintErrorMessage(response_msg[0]);
									break;
								case LCMD_READ: {	// ��������� ������� read - ��������� ���������
										bool answer = QuestionYN("�������� ������ ������������� (y) ��� ��� ��������� (n)?");
										uint messsagesQty = RequestMessagesQty(login, answer ? LCMD_UNREADED_COUNT : LCMD_MESSAGES_COUNT);	// �������� ���������� ���������
										
										if (messsagesQty > 0)
											PrintMessages(answer ? LCMD_READ_UNREADED : LCMD_READ_ALL, login);
										else
											cout << "��� " << (answer ? "������������� " : "") << "���������.\n";
									}
									break;
								case LCMD_USER_INFO:	// ��������� ������� info - ������� ���������� � ������������
									MessagesExchange(l_command_code);
									if (response_msg[0] == MSG_SUCCESS_RESULT)
										cout << "���������� � ������������:\n" << &response_msg[1] << '\n';
									else
										PrintErrorMessage(response_msg[0]);
									break;
								case LCMD_USERS_LIST:	// ��������� ������� list - ������� ������ ���� �������������
									cout << "������ �������������:\n   � #id ���\t�������\tlogin\te-mail\t���� �������.\t��������� ���������\n";
									PrintMessages(l_command_code, login);
									break;
								case LCMD_USER_DELETE:	// ��������� ������� delete - ������� ������� ������ ������������
									if (QuestionYN("���������� � ������������ ����� �������� �������. �� �������?") && 
										QuestionYN("��� �������� ������ ����� ��������. ����������� �������� ��� ���."))
										MessagesExchange(l_command_code);
										login.clear();
									break;
								case LCMD_USER_CHANGE: {	// ��������� ������� change - �������� ������� ������ ������������
										bool lcmd_loop = true;
										cout << "�� ���������� � ���� ��������� ������� ������ ������������ '" << login << "'\n";
										print_help(SUBLOCAL_COMMANDS);

										while (lcmd_loop) {
											WIN(memset(request_msg, 0, BUFFER_LENGTH))NIX(bzero(request_msg, BUFFER_LENGTH));
											console_prompt = login + ", �������� �������� ��� ������� >> ";
											request_msg_length = 1;	// ������������� ����������� ����� ���������

											switch (auto sl_command_code = get_command(SUBLOCAL_COMMANDS, console_prompt)) {	// �������� ��������� ������� � �������
												case SLCMD_CHANGE_NAME:		// ��������� ������� name		- �������� ��� ������������
												case SLCMD_CHANGE_SURNAME:	// ��������� ������� surname	- �������� ������� ������������
												case SLCMD_CHANGE_LOGIN:	// ��������� ������� log		- �������� ������� ��� (login) ������������
												case SLCMD_CHANGE_EMAIL:	// ��������� ������� email		- �������� e-mail ������������
												case SLCMD_CHANGE_PASSWORD:	// ��������� ������� pwd		- �������� ������ ������������
													SetRegData(any_text, sl_command_code);
														
													if (sl_command_code == SLCMD_CHANGE_PASSWORD) {
														CalculateSHA1(pass_sha1_hash, any_text.c_str());
														
														memcpy(&request_msg[1], &pass_sha1_hash, SHA1_HASH_LENGTH_BYTES);	// �������� ����� ���
														request_msg_length += SHA1_HASH_LENGTH_BYTES;
													}
													else
														request_msg_length += strcopy(&request_msg[1], any_text); // �������� ��������, ������������� ����� ���������

													MessagesExchange(sl_command_code);

													if (response_msg[0] == MSG_SUCCESS_RESULT) {
														switch (sl_command_code) {
															case SLCMD_CHANGE_NAME:	// ��������� ������� name - "�������� ��� ������������"
																cout << "��� ������������ '" << login << "' ���� ��������.\n\n";
																break;
															case SLCMD_CHANGE_SURNAME:	// ��������� ������� surname - "�������� ������� ������������"
																cout << "������� ������������ '" << login << "' ���� ��������.\n\n";
																break;
															case SLCMD_CHANGE_LOGIN:	// ��������� ������� log - "�������� ������� ��� (login) ������������"
																login = any_text;
																cout << "������� ��� (login) ������������ ���� �������� �� '" << login << "'.\n\n";
																break;
															case SLCMD_CHANGE_EMAIL:	// ��������� ������� email - "�������� e-mail ������������"
																cout << "����� ��. ����� ������������ '" << login << "' ��� ������.\n\n";
																break;
															case SLCMD_CHANGE_PASSWORD:	// ��������� ������� pwd - "�������� ������ ������������"
																cout << "������ ������������ '" << login << "' ��� ������.\n\n";
																break;
														}
													}
													else
														PrintErrorMessage(response_msg[0]);

													break;
												case SLCMD_HELP:	// ��������� ������� help - ������� ������� �� ��������
													print_help(SUBLOCAL_COMMANDS);
													break;
												case SLCMD_QUIT:	// ��������� ������� quit - ����� �� ����� ����
													lcmd_loop = false;
													cout << "�� ����� �� ���� ��������� ������� ������.\n\n";
													break;
												default:
													cout << MSG_ERR_CMD;
											}
										}
										console_prompt = login + ", ������� ������� >> ";	// ��������������� ����������� �������

									}	// ����� ���������� ������������ ������� change - "�������� ������� ������ ������������"
									break;
								case LCMD_HELP:	// ��������� ������� help - ������� ������� �� ��������
									print_help(LOCAL_COMMANDS);
									break;
								case LCMD_LOGOUT:	// ��������� ������� logout - �����
									request_msg_length += strcopy(&request_msg[1], login);	// �������� login, ������������� ����� ���������
									MessagesExchange(l_command_code);
									login.clear();
									break;
								default:
									cout << MSG_ERR_CMD;
							}
						}	// ����� ���������� ������������ ������
					}
					else
						PrintErrorMessage(response_msg[0]);	// ������ �����������
				}
				break;	// ���������� ������ ������
			case GCMD_HELP:	// ������� help - ������� ������� �� ��������
				print_help(GLOBAL_COMMANDS);
				break;
			case GCMD_EXIT:	// ������� exit - ������� ���
				loop = false;	// �������������� ��������� �������
				break;
			default:
				cout << MSG_ERR_CMD;
		}
	}
	return 0;
}


int ChatClient() {	// ������� ���������� ����
	while (loop) {
		FD_ZERO(&descriptors_set);
		FD_SET(server_descriptor, &descriptors_set);

		int iResult = select(0, nullptr, nullptr, &descriptors_set, &timeout);	// ��������� ����� �� ������
		if (iResult > 0) {
			WIN(cout << "\n���� ������ � ����������� � �������\n��� ������: " << WSAGetLastError() << '\n';);
			ConsoleWaitMessage();
		}

		descriptors_set.fd_count = 1;
		iResult = select(0, &descriptors_set, nullptr, nullptr, &timeout);	// ��������� ���������� ������ �� ������
		if (iResult > 0) {
			iResult = recv(server_descriptor, request_msg, BUFFER_LENGTH, 0);	// ��������� ������
			if (iResult > 0)
				RequestHandler();	// ������������ �������� ������ �� ������ � ���������� ������
			else if (iResult WIN(== SOCKET_ERROR)NIX(!= 0)) {
				WIN(cout << "��� ������: " << WSAGetLastError() << '\n';)
				cout << "����������� � ������� ��������, ������ ����� ������.\n";
				loop = false;	// �������������� ��������� �������
				break;
			}
		}
		WIN(Sleep(DELAY_MS))NIX(usleep(DELAY_MCS));
	}

	client_stop();	// ��������� ������� � �����

	return 0;
}
