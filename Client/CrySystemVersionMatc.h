#pragma once
#include "action/IAction.h"
namespace action
{
	class CrySystemVersionMatc : public IAction
	{
	public:
		virtual void Exec(std::shared_ptr<work::WorkServer> &client, const char *data, uint32_t len) override;

		static CrySystemVersionMatc	Init;
	};
}