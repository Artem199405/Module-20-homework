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
	int _count; // Количество зарегистрированных учатников чата
	vector<Participants> _participants;

	AutoLogin loginSource; // Префиксное дерево для создания новых логинов в чате

	SOCKET listenSocket; //  Сокет, который прослушивает клиентские подключения
	vector<SOCKET> participantSocket; // Клиентские сокеты

public:
	//Конструктор
	Chat(int size);

	// Деструктор
	~Chat();

	// Восстановление чата
	void restoringChat();

	// Запуск сервера
	void runServer();

	// Менеджер по работе с участниками чата
	void participantHandler(int);

	// Регистрация аккаунта нового участника чата
	void registration(int);

	// Автодопление введенного логина
	void findFreeLogin(int, string);

	// Добавление нового участника чата
	void addParticipant(string, string, string);

	// Запись в файл данных нового участника чата
	void writeParticipantsFile(string, string, string);

	// Авторизация аккаунта
	void authorization(int);

	// Вход в аккаунт
	bool signIn(string, string);

	// Определение хеш от пароля
	uint findHash(string);

	// Ввод сообщения
	void enteringMessage(int);

	// Отправка сообщения
	void sendMessage(string, string, string);
	
	// Чтение сообщения
	void readMessages(int);

	// Сравнение существующих и введенного логинов
	bool compareLogin(string);

	// Сравнение существующих и введенного имен
	bool compareName(string);

	// Получить имя пользователя по логину
	string getParticipantName(string);

	// Получить индекс пользователя в массиве _participants по имени
	int getParticipantIndex(string);

	// Получить от участника чата переменную типа int
	int getParticipantInt(int);

	// Получить от участника чата переменную типа string
	string getParticipantString(int);

	// Отправить участнику чата переменную типа int
	void sendParticipantInt(int, int);

	// Отправить участнику чата переменную типа string
	void sendParticipantString(int, string);
};