#include "stdafx.h"

#include <iostream>
#include <vector>
#include <winsock2.h>

#include <data/Keepalive.pb.h>

#pragma comment(lib, "ws2_32.lib")
#define PORT 12345
#define IP_ADDRESS "43.241.48.144"

const int HeardSize = sizeof(unsigned int) + sizeof(unsigned int);

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA Ws;
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		std::cout << "Init Windows Socket Failed::" << GetLastError() << std::endl;
		return -1;
	}

	auto connecter = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connecter == INVALID_SOCKET)
	{
		std::cout << "Create Socket Failed::" << GetLastError() << std::endl;
		return -1;
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	server_addr.sin_port = htons(PORT);
	memset(server_addr.sin_zero, 0x00, 8);

	auto status = connect(connecter, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (status == SOCKET_ERROR)
	{
		std::cout << "Connect Error::" << GetLastError() << std::endl;
		return -1;
	}
	else
	{
		std::cout << "连接成功!" << std::endl;
	}

	while (true)
	{
		data::KeepaliveRequest req;
		data::KeepaliveResponse resp;

		auto len = req.ByteSize() + HeardSize;
		std::vector<char> buff;
		buff.resize(len);
		*reinterpret_cast<uint32_t *>(buff.data()) = htonl(len);
		*reinterpret_cast<uint32_t *>(buff.data() + sizeof(uint32_t)) = htonl(4);

		req.SerializePartialToArray(buff.data() + sizeof(uint32_t) + sizeof(uint32_t), req.ByteSize());

		auto status = send(connecter, buff.data(), len, 0);
		if (status <= 0)
		{
			break;
		}
		buff.resize(HeardSize);
		status = recv(connecter, buff.data(), buff.size(), 0);
		if (status <= 0)
		{
			break;
		}
		auto recv_len = ntohl(*reinterpret_cast<uint32_t *>(buff.data()));
		auto proto_id = ntohl(*reinterpret_cast<uint32_t *>(buff.data() + sizeof(uint32_t)));
		buff.resize(recv_len - HeardSize);
		if (buff.size())
		{
			status = recv(connecter, buff.data(), buff.size(), 0);
			if (status <= 0)
			{
				break;
			}
		}

		resp.ParseFromArray(buff.data(), buff.size());
		Sleep(6000);
		std::cout << "保活" << std::endl;
	}
	std::cout << "客户端退出" << std::endl;
	closesocket(connecter);
	WSACleanup();
	getchar();
	return 0;
}

