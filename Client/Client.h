#pragma once
#include <evpp/event_loop_thread.h>
#include <evpp/tcp_conn.h>
#include <evpp/tcp_client.h>
#include <evpp/libevent.h>
#include <unordered_map>

namespace work
{
	class WorkServer;
}

namespace action
{
	class IActionDB;
}

class CClient
{
public:
	CClient();
	virtual ~CClient();
public:
	/* �����ͻ��� */
	bool								Start();
	/* ֹͣ�ͻ��� */
	void								Stop();
	/* ��Ϣ  �ص� */
	void								OnMessage(const evpp::TCPConnPtr& conn, evpp::Buffer* msg);
	/* ���ӵ��¼� */
	void								OnConnection(const evpp::TCPConnPtr& conn);
	void								AddServer(uint64_t id, std::shared_ptr<work::WorkServer>& client);
	/*����idɾ���ͻ���*/
	bool								DelServer(uint64_t id);
	std::shared_ptr<work::WorkServer>	GetServer(uint64_t id = 0);
	action::IActionDB*					GetActions();
	void								SetMainForm(CDialogEx* MainForm){ m_MainForm = MainForm; }
	CDialogEx*							GetMainForm(){ return m_MainForm; }
private:
	//����id���ҷ���˶���
	std::unordered_map<uint64_t, std::shared_ptr<work::WorkServer>>		m_servers;
	std::mutex															m_server_mutex;
	//std::unique_ptr<evpp::EventLoop>									m_loop = nullptr;
	std::unique_ptr<evpp::EventLoopThread>								m_loop = nullptr;
	std::unique_ptr<evpp::TCPClient>									m_tcp_client = nullptr;
	CDialogEx*															m_MainForm;
};