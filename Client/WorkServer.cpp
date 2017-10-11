#include "stdafx.h"
#include "Client.h"
#include "WorkServer.h"
#include "action/ActionID.h"
#include "action/IActionDB.h"
#include "action/IAction.h"
#include <data/VersionMatch.pb.h>
#include "CryFileHelper.h"
#include <thread>

namespace work
{
	const uint32_t work::WorkServer::HeardSize = sizeof(uint32_t) + sizeof(uint32_t);

	WorkServer::WorkServer()
	{
		m_message_id = 0;
		m_message_len = 0;
	}


	WorkServer::~WorkServer()
	{

	}
	void WorkServer::Read(evpp::Buffer * msg)
	{
		while (msg->length() > 0)
		{
			//¶ÁÈ¡°üÍ·
			if (m_message_id == 0)
			{
				if (msg->length() < HeardSize)
				{
					break;
				}
				m_message_len = msg->ReadInt32();
				m_message_len -= HeardSize;
				m_message_id = msg->ReadInt32();
			}

			if (msg->size() < m_message_len)
			{
				break;
			}

			auto action = GetAction(m_message_id);
			if (action != nullptr)
			{
				action->Exec(shared_from_this(), msg->data(), m_message_len);
			}

			msg->Skip(m_message_len);
			m_message_id = 0;
		}
	}
	void WorkServer::Send(uint32_t message_id, const ::google::protobuf::Message &message)
	{
		auto len = message.ByteSize() + HeardSize;
		if (m_send_buffer.capacity() < len)
		{
			m_send_buffer.resize(len);
		}

		*reinterpret_cast<uint32_t *>(const_cast<char *>(m_send_buffer.data())) = htonl(len);
		*reinterpret_cast<uint32_t *>(const_cast<char *>(m_send_buffer.data()) + sizeof(uint32_t)) = htonl(message_id);

		message.SerializePartialToArray(const_cast<char *>(m_send_buffer.data()) + HeardSize, message.ByteSize());

		m_tcp_conn->Send(m_send_buffer.data(), len);
	}

	action::IAction *WorkServer::GetAction(uint32_t proto_id)
	{
		return m_client->GetActions()->GetAction(proto_id);
	}
	void WorkServer::CheckUpdate()
	{
		CryFileHelper FileHelper;
		CHAR lpszClient[48] = { 0 }, lpszLibrary[48] = { 0 };
		FileHelper.MakeFile("./Client.bin", lpszClient);
		FileHelper.MakeFile("./Game.dll", lpszLibrary);
		data::VersionMatchRequest req;
		req.mutable_file_md5s()->insert({ "Client.bin", lpszClient });
		req.mutable_file_md5s()->insert({ "Game.dll", lpszLibrary });
		Send(action::ActionID::VersionMatch, req);
	}
}