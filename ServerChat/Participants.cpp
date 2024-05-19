#include <iostream>
#include "Participants.h"

Participants:: Participants() : _count(0)
{
    vector<Messages> _messages(1);
}

Participants::~Participants()
{
}

void Participants::recordMessage(string name, string text)
{
    Messages message;

    message.setSender(name);
    message.setText(text);
    
    vector<Messages>::iterator it = _messages.begin() + _count;
    vector<Messages>::iterator newIt = _messages.insert(it, message);
    
	_count++;
}

string Participants::showMessageSender(int i)
{
    return _messages[i].getSender();
}

string Participants::showMessageText(int i)
{
    return _messages[i].getText();
}

void Participants::writeMessagesFile(string receiver)
{
    fstream messages_file = fstream("Messages.txt", ios::in | ios::out);

    filesystem::permissions("Messages.txt",
        filesystem::perms::group_all | filesystem::perms::others_all,
        filesystem::perm_options::remove);

    if (!messages_file)
        messages_file = fstream("Messages.txt", ios::in | ios::out | ios::trunc);

    for (int i = 0; i < _count; i++)
    {
        if (messages_file)
        {
            messages_file.seekp(0, std::ios_base::end);
            messages_file << receiver << ' ' << _messages[i].getSender() <<
                ' ' << _messages[i].getText() << endl;
        }
    }
}

void Participants::setCount(int count)
{
    _count = count;
}

void Participants::setLogin(string login)
{
	_login = login;
}

void Participants::setPassword(string password)
{
	_password = password;
}

void Participants::setName(string name)
{
	_name = name;
}

void Participants::setPasswordHash(string password)
{
    const char* charPassword = password.c_str();
    int length = strlen(charPassword);

    _password_hash = sha1(charPassword, (uint)length);
}

int Participants::getCount()
{
    return _count;
}

string Participants::getLogin()
{
	return _login;
}

string Participants::getPassword()
{
	return _password;
}

string Participants::getName()
{
	return _name;
}

uint Participants::getPasswordHash()
{
    return *_password_hash;
}