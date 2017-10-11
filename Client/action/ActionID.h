#pragma once

#include <cstdint>

namespace action
{

	struct ActionID
	{
		static const uint32_t		VersionMatch			= 1;
		static const uint32_t		AdministorLogin			= 2;
		static const uint32_t		GetUserNumber			= 3;
		static const uint32_t		Keepalive				= 4;

		static const uint32_t		AccountRegister			= 1000;
		static const uint32_t		AccountLogin			= 1001;
		static const uint32_t		AccountModify			= 1002;
		static const uint32_t		AccounTretrieve			= 1003;
		static const uint32_t		AccounRecharge			= 1004;
		static const uint32_t		AccounCardQuer			= 1005;

		static const uint32_t		ManagerCardAdd			= 2000;
		static const uint32_t		ManagerCardQuery		= 2001;
	};
}
