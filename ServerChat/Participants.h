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
	int _count; // Количество присланных сообщений
	string _login, _password, _name;
	uint* _password_hash;
	vector<Messages> _messages;

public:
	//Конструктор
	Participants();

	// Деструктор
	~Participants();

	// Добавление нового сообщения
	void recordMessage(string, string);

	// Чтение присланных сообщений (отправитель)
	string showMessageSender(int);

	// Чтение присланных сообщений (текст)
	string showMessageText(int);

	// Запись в файл присланные (непрочитанные) сообщения
	void writeMessagesFile(string);

	// Сеттеры
	void setCount(int);
	
	void setLogin(string);

	void setPassword(string);

	void setName(string);

	void setPasswordHash(string);

	// Геттеры
	int getCount();
	
	string getLogin();

	string getPassword();

	string getName();

	uint getPasswordHash();
};