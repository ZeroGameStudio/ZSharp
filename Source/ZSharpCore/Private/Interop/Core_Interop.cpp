// Copyright Zero Games. All Rights Reserved.


#include "Core_Interop.h"

DEFINE_LOG_CATEGORY_STATIC(LogZSharpScriptCore, Log, All)

#define LOG_CASE(Level, Verbosity) case Level: { UE_LOG(LogZSharpScriptCore, Verbosity, TEXT("%s"), message); break; }

void ZSharp::FZCore_Interop::CoreLog(uint8 level, const TCHAR* message)
{
#define Category LogZSharpScriptCore
	
	switch (level)
	{
		LOG_CASE(1, Fatal)
		LOG_CASE(2, Error)
		LOG_CASE(3, Warning)
		LOG_CASE(4, Display)
		LOG_CASE(5, Log)
		LOG_CASE(6, Verbose)
		LOG_CASE(7, VeryVerbose)
	default:
		{
			break;
		}
	}

#undef Category
}

#undef LOG_CASE


