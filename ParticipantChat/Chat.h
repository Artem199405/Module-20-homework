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
	string nameParticipant; // Имя авторизованного участника чата из массива _participants

	SOCKET connectSocket; // Сокет для подключения к серверу

public:
	//Конструктор
	Chat();

	// Деструктор
	~Chat();

	// Установить соединение с сервером
	void connectServer();

	// Регистрация аккаунта нового участника чата
	void registration();

	// Авторизация аккаунта
	void authorization();

	// Ввод сообщения
	void enteringMessage();

	// Чтение сообщения
	void readMessages();

	// Получить от сервера переменную типа int
	int getServerInt();

	// Получить от сервера переменную типа string
	string getServerString();

	// Отправить на сервер переменную типа int
	void sendServerInt(int);

	// Отправить на сервер переменную типа string
	void sendServerString(string);
};