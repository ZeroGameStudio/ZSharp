// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZLog_Interop
	{
		static void Log(const TCHAR* category, ELogVerbosity::Type verbosity, const TCHAR* message);
	};
}


