#pragma once
#include "WorkServer.h"
#include "Client.h"
#include "action/ActionID.h"
namespace work
{
	class WorkServer;
}
namespace action
{
	class IAction
	{
	public:
		virtual ~IAction() = default;

		virtual void				Exec(std::shared_ptr<work::WorkServer> &client, const char *data, uint32_t len) = 0;
	protected:
		IAction() = default;
		IAction(const IAction &) = default;
		IAction &operator=(const IAction &) = default;
	};
}