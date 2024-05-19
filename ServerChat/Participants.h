#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "Messages.h"
#include "Sha1.h"

using namespace std;

class Participants
{
private:
	int _count; // ���������� ���������� ���������
	string _login, _password, _name;
	uint* _password_hash;
	vector<Messages> _messages;

public:
	//�����������
	Participants();

	// ����������
	~Participants();

	// ���������� ������ ���������
	void recordMessage(string, string);

	// ������ ���������� ��������� (�����������)
	string showMessageSender(int);

	// ������ ���������� ��������� (�����)
	string showMessageText(int);

	// ������ � ���� ���������� (�������������) ���������
	void writeMessagesFile(string);

	// �������
	void setCount(int);
	
	void setLogin(string);

	void setPassword(string);

	void setName(string);

	void setPasswordHash(string);

	// �������
	int getCount();
	
	string getLogin();

	string getPassword();

	string getName();

	uint getPasswordHash();
};