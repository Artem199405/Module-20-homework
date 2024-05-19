#pragma once

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include "Participants.h"
#include "AutoLogin.h"

using namespace std;

#define port 7777

#pragma warning(disable: 4996)

class Chat
{
private:
	int _count; // ���������� ������������������ ��������� ����
	vector<Participants> _participants;

	AutoLogin loginSource; // ���������� ������ ��� �������� ����� ������� � ����

	SOCKET listenSocket; //  �����, ������� ������������ ���������� �����������
	vector<SOCKET> participantSocket; // ���������� ������

public:
	//�����������
	Chat(int size);

	// ����������
	~Chat();

	// �������������� ����
	void restoringChat();

	// ������ �������
	void runServer();

	// �������� �� ������ � ����������� ����
	void participantHandler(int);

	// ����������� �������� ������ ��������� ����
	void registration(int);

	// ������������ ���������� ������
	void findFreeLogin(int, string);

	// ���������� ������ ��������� ����
	void addParticipant(string, string, string);

	// ������ � ���� ������ ������ ��������� ����
	void writeParticipantsFile(string, string, string);

	// ����������� ��������
	void authorization(int);

	// ���� � �������
	bool signIn(string, string);

	// ����������� ��� �� ������
	uint findHash(string);

	// ���� ���������
	void enteringMessage(int);

	// �������� ���������
	void sendMessage(string, string, string);
	
	// ������ ���������
	void readMessages(int);

	// ��������� ������������ � ���������� �������
	bool compareLogin(string);

	// ��������� ������������ � ���������� ����
	bool compareName(string);

	// �������� ��� ������������ �� ������
	string getParticipantName(string);

	// �������� ������ ������������ � ������� _participants �� �����
	int getParticipantIndex(string);

	// �������� �� ��������� ���� ���������� ���� int
	int getParticipantInt(int);

	// �������� �� ��������� ���� ���������� ���� string
	string getParticipantString(int);

	// ��������� ��������� ���� ���������� ���� int
	void sendParticipantInt(int, int);

	// ��������� ��������� ���� ���������� ���� string
	void sendParticipantString(int, string);
};