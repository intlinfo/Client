#pragma once

#include <cstdint>
#include <memory>

namespace action
{
	class IAction;

	class IActionDB
	{
	public:
		virtual ~IActionDB() = default;

		virtual bool				Init() = 0;
		virtual bool				Uninit() = 0;
		virtual IAction				*GetAction(uint32_t proto_id) = 0;
	};
}