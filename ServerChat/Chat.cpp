#include "Chat.h"

Chat::Chat(int size) : _count(0)
{
    vector<Participants> _participants(size - 1);
    vector<SOCKET> clientSocket(size - 1);

    loginSource.insert("Log1"); loginSource.insert("Log2"); loginSource.insert("Log3");
    loginSource.insert("Login1"); loginSource.insert("Login2"); loginSource.insert("Login3");
    loginSource.insert("log1"); loginSource.insert("log2"); loginSource.insert("log3");
    loginSource.insert("login1"); loginSource.insert("login2"); loginSource.insert("login3");
}

Chat::~Chat()
{
    closesocket(listenSocket);
}

void Chat::restoringChat()
{
    fstream participant_file = fstream("Participants.txt", ios::in | ios::out);

    if (participant_file)
    {
        participant_file.seekp(0, ios_base::beg);
        while (!participant_file.eof())
        {
            string login, password, name;

            participant_file >> login;
            participant_file >> password;
            participant_file >> name;
            if (login != "" && password != "" && name != "")
                addParticipant(login, password, name);
        }
    }

    fstream messages_file = fstream("Messages.txt", ios::in | ios::out);

    if (messages_file)
    {
        messages_file.seekp(0, ios_base::beg);
        while (!messages_file.eof())
        {
            string receiver, sender, text;

            messages_file >> receiver;
            messages_file >> sender;
            messages_file >> text;
            if (receiver != "" && sender != "" && text != "")
            {
                for (int i = 0; i < _count; i++)
                {
                    if (receiver == _participants[i].getName())
                    {
                        _participants[i].recordMessage(sender, text);
                    }
                }
            }
        }

        messages_file = fstream("Messages.txt", ios::in | ios::out | ios::trunc);
    }
}

void Chat::runServer()
{
    // WSAStartup
    WSAData wsaData;
    // Запрашиваемая версия библиотеки winsock
    WORD DLLVersion = MAKEWORD(2, 1);

    int iResult;

    // Инициализировать Winsock
    iResult = WSAStartup(DLLVersion, &wsaData);
    if (iResult != 0)
    {
        cout << ("WSAStartup failed with error: ", iResult);
        system("pause");
    }

    // Адрес сокета
    SOCKADDR_IN address;
    int sizeAddress = sizeof(address);
    // Зададим номер IP для связи
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Зададим номер порта для связи
    address.sin_port = htons(port);
    // Используем IPv4
    address.sin_family = AF_INET;

    // Создаем сокет для сервера, который будет прослушивать клиентские подключения
    listenSocket = socket(AF_INET, SOCK_STREAM, NULL);
    if (listenSocket == INVALID_SOCKET)
    {
        cout << ("Socket failed with error: ", WSAGetLastError());
        system("pause");
    }
    // Настройка прослушивающего сокета TCP
    iResult = bind(listenSocket, (SOCKADDR*)&address, sizeAddress);
    if (iResult == SOCKET_ERROR)
    {
        cout << ("Bind failed with error: ", WSAGetLastError());
        system("pause");
    }
    // Поставим сервер на прием данных 
    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        cout << ("Listen failed with error: ", WSAGetLastError());
        system("pause");
    }

    SOCKET connection;
    while (true)
    {
        // Принять клиентский сокет
        connection = accept(listenSocket, (SOCKADDR*)&address, &sizeAddress);

        if (connection == INVALID_SOCKET)
        {
            std::cout << ("accept failed with error: ", WSAGetLastError());
            system("pause");
        }
        else
        {
            std::cout << "Client Connected!" << endl;

            participantSocket.push_back(connection);

            participantHandler(participantSocket.size() - 1);
            //CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)participantHandler,
            //    (LPVOID)(participantSocket.size() - 1), NULL, NULL);
        }
    }
}

void Chat::participantHandler(int i)
{
    int action;
    
    do
    {
        action = getParticipantInt(i);
        if (action == 1)
            // Регистрация аккаунтов
            registration(i);
        else if (action == 2)
        {
            // Вход в аккаунт
            authorization(i);

            do
            {
                action = getParticipantInt(i);
                if (action == 1)
                    // Ввод сообщения
                    enteringMessage(i);
                else if (action == 2)
                    // Чтение сообщения
                    readMessages(i);
                else if (action == 3)
                {

                }
            }
            while (action != 3);

            action = 2;
        }
        else if (action == 3)
        {

        }
    }
    while (action != 3);
}

void Chat::registration(int i)
{
    string login, password, name;
    bool key = true;

    do
    {
        int action;

        recv(participantSocket[i], (char*)&action, sizeof(int), NULL);
        if (action == 1)
        {
            string keyword = getParticipantString(i);
            
            findFreeLogin(i, keyword);
        }
        login = getParticipantString(i);
        key = compareLogin(login);
        if (key == true)
        {
            sendParticipantInt(i, 1);
            sendParticipantString(i, "A user with this login already exists. Come up with a new login.");
        }
        else
        {
            sendParticipantInt(i, 0);
            sendParticipantString(i, "Good login.");
        }
    }
    while (key == true);

    password = getParticipantString(i);

    do
    {
        name = getParticipantString(i);
        key = compareName(name);
        if (key == true)
        {
            sendParticipantInt(i, 1);
            sendParticipantString(i, "A user with this username already exists. Come up with a new username.");
        }
        else
        {
            sendParticipantInt(i, 0);
            sendParticipantString(i, "Good username.");
        }
    }
    while (key == true);

    addParticipant(login, password, name);
    writeParticipantsFile(login, password, name);
}

void Chat::findFreeLogin(int i, string keyword)
{
    string res;

    loginSource.findPrefixWords(keyword, &res);

    if (res.at(0) == ' ')
    {
        sendParticipantString(i, "No words with matching keyword found ");
    }
    else
    {
        string newLogin, logins;

        logins = "Words with keyword: ";

        for (size_t i = 0; i < res.length(); i++)
        {

            if (res.at(i) == ' ')
            {
                bool key = compareLogin(newLogin);
                if (!key)
                    logins.append(newLogin + " ");

                newLogin.clear();
            }
            else
                newLogin += res.at(i);
        }

        sendParticipantString(i, logins);
    }

    cout << endl;
}

void Chat::addParticipant(string login, string password, string name)
{
    Participants participant;

    participant.setLogin(login);
    participant.setPassword(password);
    participant.setName(name);
    participant.setPasswordHash(password);

    vector<Participants>::iterator it = _participants.begin() + _count;
    vector<Participants>::iterator newIt = _participants.insert(it, participant);

    _count++;
}

void Chat::writeParticipantsFile(string login, string password, string name)
{
    fstream participant_file = fstream("Participants.txt", ios::in | ios::out);

    filesystem::permissions("Participants.txt",
        filesystem::perms::group_all | filesystem::perms::others_all,
        filesystem::perm_options::remove);

    if (!participant_file)
        participant_file = fstream("Participants.txt", ios::in | ios::out | ios::trunc);

    if (participant_file)
    {
        participant_file.seekp(0, std::ios_base::end);
        participant_file << login << ' ' << password << ' ' << name << endl;
    }
    else
    {
        cout << "Could not open file Participants.txt !" << endl;
    }
}

void Chat::authorization(int i)
{
    string login, password;
    bool key = true;

    do
    {
        login = getParticipantString(i);
        password = getParticipantString(i);
        key = signIn(login, password);
        if (key == true)
        {
            sendParticipantInt(i, 1);
            sendParticipantString(i, "You are logged in to your account.");
        }
        else
        {
            sendParticipantInt(i, 0);
            sendParticipantString(i, "You have entered an incorrect login or password. Try again.");
        }
    }
    while (key == false);

    sendParticipantString(i, getParticipantName(login));
}

bool Chat::signIn(string login, string password)
{
    uint hash = findHash(password);

    for (int i = 0; i < _count; i++)
    {
        if (login == _participants[i].getLogin())
        {
            // Первый вариант
            /*if (password == _participants[i].getPassword())
            {
                return true;
            }
            else
            {
                return false;
            }*/

            // Второй вариант
            if (hash == _participants[i].getPasswordHash())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    return false;
}

uint Chat::findHash(string password)
{
    const char* charPassword = password.c_str();
    int length = strlen(charPassword);

    uint* hash = sha1(charPassword, (uint)length);

    return *hash;
}

void Chat::enteringMessage(int i)
{
    string recipientName, nameSender, text;
    bool key = false;

    do
    {
        recipientName = getParticipantString(i);
        key = compareName(recipientName);
        if (key == true)
            sendParticipantInt(i, 1);
        else
        {
            sendParticipantInt(i, 0);
            sendParticipantString(i, "A user with that name was not found. Try again.");
        }
    }
    while (key == false);

    nameSender = getParticipantString(i);
    text = getParticipantString(i);

    sendMessage(nameSender, recipientName, text);
}

void Chat::sendMessage(string nameSender, string recipientName, string text)
{
    int iSender = getParticipantIndex(nameSender);

    if (recipientName == "all")
    {
        for (int i = 0; i < _count; i++)
        {
            if (i != iSender)
            {
                _participants[i].recordMessage(_participants[iSender].getName(), text);
            }
        }
    }
    else
    {
        for (int i = 0; i < _count; i++)
        {
            if (recipientName == _participants[i].getName())
            {
                _participants[i].recordMessage(_participants[iSender].getName(), text);
                break;
            }
        }
    }
}

void Chat::readMessages(int index)
{
    string recipientName = getParticipantString(index);
    int iRecipient = getParticipantIndex(recipientName);
    int countMessages = _participants[iRecipient].getCount();

    if (countMessages == 0)
        sendParticipantString(index, "You don't have any unread messages.");
    else if (countMessages == 1)
        sendParticipantString(index, "You have one unread message.");
    else
        sendParticipantString(index, "You have " + to_string(countMessages) + " unread messages.");

    sendParticipantInt(index, countMessages);
    for (int i = 0; i < countMessages; i++)
    {
        sendParticipantString(index, "Sender: " + _participants[iRecipient].showMessageSender(i));
        sendParticipantString(index, "Text: " + _participants[iRecipient].showMessageText(i));
    }

    _participants[iRecipient].setCount(0);
}

bool Chat::compareLogin(string login)
{
    for (int i = 0; i < _count; i++)
    {
        if (login == _participants[i].getLogin())
            return true;
    }

    return false;
}

bool Chat::compareName(string name)
{
    if (name == "all")
        return true;

    for (int i = 0; i < _count; i++)
    {
        if (name == _participants[i].getName())
            return true;
    }

    return false;
}

string Chat::getParticipantName(string login)
{
    for (int i = 0; i < _count; i++)
    {
        if (login == _participants[i].getLogin())
        {
            return _participants[i].getName();
        }
    }
}

int Chat::getParticipantIndex(string name)
{
    for (int i = 0; i < _count; i++)
    {
        if (name == _participants[i].getName())
        {
            return i;
        }
    }
}

int Chat::getParticipantInt(int i)
{
    int j;
    
    recv(participantSocket[i], (char*)&j, sizeof(int), NULL);

    return j;
}

string Chat::getParticipantString(int i)
{
    int sizeWord;

    recv(participantSocket[i], (char*)&sizeWord, sizeof(int), NULL);

    char* word = new char[sizeWord + 1];
    word[sizeWord] = '\0';

    recv(participantSocket[i], word, sizeWord, NULL);

    return word;
}

void Chat::sendParticipantInt(int i, int j)
{
    send(participantSocket[i], (char*)&j, sizeof(int), NULL);
}

void Chat::sendParticipantString(int i, string word)
{
    int sizeWord = word.size();

    send(participantSocket[i], (char*)&sizeWord, sizeof(int), NULL);
    send(participantSocket[i], word.c_str(), sizeWord, NULL);
}