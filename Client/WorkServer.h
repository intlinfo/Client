#pragma once
#include <google/protobuf/message.h>
#include <evpp/buffer.h>
#include <evpp/tcp_conn.h>
#include <event2/event.h>
#include <memory>
class CClient;

namespace action
{
	class IAction;
}

namespace work
{
	class WorkServer : public std::enable_shared_from_this<work::WorkServer>
	{
	public:
		WorkServer();
		~WorkServer();
	public:
		void						Read(evpp::Buffer * msg);
		void						Send(uint32_t message_id, const ::google::protobuf::Message &message);
		void						SetTcpConn(evpp::TCPConnPtr tcp_conn) { m_tcp_conn = tcp_conn; }
		void						SetServer(CClient *client) { m_client = client; }
		CClient						*GetClient() { return m_client; }
		void						Close() { m_tcp_conn->Close(); }
		uint64_t					Id() const { return m_tcp_conn->id(); }

		action::IAction *			GetAction(uint32_t proto_id);
		void						CheckUpdate();
		CDialogEx*					GetMainForm(){ return m_client->GetMainForm(); }
		void						SetWnd(HWND hwnd) { m_hwnd = hwnd; }
		HWND						GetWnd() const { return m_hwnd; }
	private:
		CClient						*m_client = nullptr;
		evpp::TCPConnPtr			m_tcp_conn;

		uint32_t					m_message_len = 0;
		uint32_t					m_message_id = 0;

		std::string					m_send_buffer;
		static const uint32_t		HeardSize;
		HWND						m_hwnd;
	};
}