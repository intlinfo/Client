#pragma once

#include "IActionDB.h"


#include <unordered_map>

namespace action
{

	class ActionDB : public IActionDB
	{
	public:
		ActionDB();
		~ActionDB();

		bool				Init() override;
		bool				Uninit() override;
		IAction				*GetAction(uint32_t proto_id) override;

	private:
		std::unordered_map<uint32_t, IAction *>	m_actions;
	};

}