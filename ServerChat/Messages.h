#pragma once

#include <string>

using namespace std;

class Messages
{
private:
	string _sender, _text;

public:
	//�����������
	Messages();

	// ����������
	~Messages();

	// �������
	void setSender(string);

	void setText(string);

	// �������
	string getSender();

	string getText();
};