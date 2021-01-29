#pragma once

//https://github.com/graeme-hill/crossguid
#include "../../dependency/crossguid/include/crossguid/guid.hpp"

namespace doom
{
	
	using D_UUID = xg::Guid;
	inline D_UUID GenerateUUID()
	{
		return xg::newGuid();
	}
}