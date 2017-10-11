#include "stdafx.h"
#include "ActionDB.h"
#include "action/ActionID.h"
#include "CryMainForm.h"
#include "CrySystemRegisterForm.h"
#include "CrySystemRechargeForm.h"
#include "CrySystemVersionMatc.h"
#include "CrySystemFindWindowForm.h"

action::ActionDB::ActionDB()
{
}


action::ActionDB::~ActionDB()
{
	Uninit();
}

bool action::ActionDB::Init()
{
	m_actions.insert({ ActionID::VersionMatch,		&CrySystemVersionMatc::Init				});
	m_actions.insert({ ActionID::Keepalive,			CrySystemFindWindowForm::GetInstance()	});
	m_actions.insert({ ActionID::AccountLogin,		CryMainForm::GetInstance()				});
	m_actions.insert({ ActionID::AccountRegister,	CrySystemRegisterForm::GetInstance()	});
	m_actions.insert({ ActionID::AccounRecharge,	CrySystemRechargeForm::GetInstance()	});
	return true;
}

bool action::ActionDB::Uninit()
{
	m_actions.clear();
	return true;
}

action::IAction *action::ActionDB::GetAction(uint32_t proto_id)
{
	auto iter = m_actions.find(proto_id);
	if (iter == m_actions.end())
	{
		return nullptr;
	}
	return iter->second;
}
