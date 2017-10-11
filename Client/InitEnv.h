#pragma once

#include <import/Protobuf.hpp>

class InitEnv
{
public:
	InitEnv()
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
	}

	~InitEnv()
	{
		WSACleanup();
	}
private:
	import::Protobuf				m_Protobuf;
};