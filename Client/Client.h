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
	/* 启动客户端 */
	bool								Start();
	/* 停止客户端 */
	void								Stop();
	/* 消息  回调 */
	void								OnMessage(const evpp::TCPConnPtr& conn, evpp::Buffer* msg);
	/* 连接等事件 */
	void								OnConnection(const evpp::TCPConnPtr& conn);
	void								AddServer(uint64_t id, std::shared_ptr<work::WorkServer>& client);
	/*根据id删除客户端*/
	bool								DelServer(uint64_t id);
	std::shared_ptr<work::WorkServer>	GetServer(uint64_t id = 0);
	action::IActionDB*					GetActions();
	void								SetMainForm(CDialogEx* MainForm){ m_MainForm = MainForm; }
	CDialogEx*							GetMainForm(){ return m_MainForm; }
private:
	//根据id查找服务端对象
	std::unordered_map<uint64_t, std::shared_ptr<work::WorkServer>>		m_servers;
	std::mutex															m_server_mutex;
	//std::unique_ptr<evpp::EventLoop>									m_loop = nullptr;
	std::unique_ptr<evpp::EventLoopThread>								m_loop = nullptr;
	std::unique_ptr<evpp::TCPClient>									m_tcp_client = nullptr;
	CDialogEx*															m_MainForm;
};