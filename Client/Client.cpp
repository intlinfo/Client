#include "stdafx.h"
#include "Client.h"
#include "WorkServer.h"
#include "action/ActionDB.h"


CClient::CClient()
{

}

CClient::~CClient()
{
	
}

void CClient::OnMessage(const evpp::TCPConnPtr& conn, evpp::Buffer* msg)
{
	auto work_server = GetServer(conn->id());

	if (work_server)
	{
		work_server->Read(msg);
	}
}

void CClient::OnConnection(const evpp::TCPConnPtr& conn)
{
	if (conn->status() == evpp::TCPConn::Status::kDisconnected)
	{
		if (m_MainForm)
		{
			m_MainForm->GetDlgItem(IDC_BUTTON_OK)->EnableWindow(FALSE);
			m_MainForm->GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(FALSE);
			m_MainForm->GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(FALSE);
			m_MainForm->EndDialog(IDOK);
			m_MainForm->MessageBox(_T("与服务器连接失败"), _T("Disconnected"), MB_ICONINFORMATION);

		}
		LOG_WARN << "create a new connection fail from " << conn->remote_addr();
	}
	if (conn->status() == evpp::TCPConn::Status::kConnected)
	{
		auto work_server = std::make_shared<work::WorkServer>();
		if (!work_server)
		{
			conn->Close();
			return;
		}
		work_server->SetServer(this);
		work_server->SetTcpConn(conn);
		work_server->CheckUpdate();
		AddServer(work_server->Id(), work_server);
		{
			if (m_MainForm)
			{
				m_MainForm->SetWindowText("正在检查更新...");
			}
		}
		LOG_WARN << "Accept a new connection from " << conn->remote_addr();
	}
	if (conn->status() == evpp::TCPConn::Status::kDisconnecting)
	{
		DelServer(conn->id());
		LOG_WARN << "Disconnected from " << conn->remote_addr();
	}
}

bool CClient::Start()
{
	bool bRet = false;

	google::InitGoogleLogging("Client");
#ifndef _DEBUG
	google::SetLogDestination(google::GLOG_INFO, "./log/");
#else
	google::SetLogDestination(google::GLOG_INFO, "./log/Client-");
#endif
	google::InstallFailureSignalHandler();
	FLAGS_logtostderr = false;
	FLAGS_colorlogtostderr = false;
	FLAGS_v = 3;
	FLAGS_stop_logging_if_full_disk = true;

	m_loop = std::make_unique<evpp::EventLoopThread>();
	if (m_loop->Start(true))
	{
		m_tcp_client = std::make_unique<evpp::TCPClient>(m_loop->loop(), "192.168.1.2:12345", "Client");
		if (m_tcp_client == nullptr)
		{
			bRet = false;
		}
		m_tcp_client->SetMessageCallback(std::bind(&CClient::OnMessage, this, std::placeholders::_1, std::placeholders::_2));
		m_tcp_client->SetConnectionCallback(std::bind(&CClient::OnConnection, this, std::placeholders::_1));
		// 关闭自动重连
		m_tcp_client->set_auto_reconnect(false);
		m_tcp_client->Connect();
		
		bRet = true;
	}
	return bRet;
}
void CClient::Stop()
{
	if (m_tcp_client)
	{
		m_tcp_client->Disconnect();
		if (m_loop)
		{
			m_loop->Stop(true);
			m_tcp_client.reset(nullptr);
			m_loop.reset(nullptr);
			assert(evpp::GetActiveEventCount() == 0);
		}
	}
	google::ShutdownGoogleLogging();
}

void CClient::AddServer(uint64_t id, std::shared_ptr<work::WorkServer>& client)
{
	std::lock_guard<std::mutex> guard(m_server_mutex);

	m_servers.insert({ id, client });
}
bool CClient::DelServer(uint64_t id)
{
	std::lock_guard<std::mutex> guard(m_server_mutex);

	auto iter = m_servers.find(id);

	if (iter != std::end(m_servers))
	{
		m_servers.erase(iter);
	}
	return m_servers.empty();
}
std::shared_ptr<work::WorkServer> CClient::GetServer(uint64_t id)
{
	std::lock_guard<std::mutex> guard(m_server_mutex);
	auto iter = m_servers.find(id);
	if (iter == std::end(m_servers))
	{
		return nullptr;
	}

	return iter->second;
}
action::IActionDB * CClient::GetActions()
{
	thread_local static action::ActionDB *m_actions = nullptr;
	if (!m_actions)
	{
		m_actions = new action::ActionDB;
		if (!m_actions->Init())
		{
			if (m_actions)
			{
				delete m_actions;
			}
			m_actions = nullptr;
		}
	}
	return m_actions;
}