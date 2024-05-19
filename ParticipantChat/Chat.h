#pragma once

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdio>

using namespace std;

#define port 7777

#pragma warning(disable: 4996)

class Chat
{
private:
	string nameParticipant; // ��� ��������������� ��������� ���� �� ������� _participants

	SOCKET connectSocket; // ����� ��� ����������� � �������

public:
	//�����������
	Chat();

	// ����������
	~Chat();

	// ���������� ���������� � ��������
	void connectServer();

	// ����������� �������� ������ ��������� ����
	void registration();

	// ����������� ��������
	void authorization();

	// ���� ���������
	void enteringMessage();

	// ������ ���������
	void readMessages();

	// �������� �� ������� ���������� ���� int
	int getServerInt();

	// �������� �� ������� ���������� ���� string
	string getServerString();

	// ��������� �� ������ ���������� ���� int
	void sendServerInt(int);

	// ��������� �� ������ ���������� ���� string
	void sendServerString(string);
};