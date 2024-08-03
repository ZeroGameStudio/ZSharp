// Copyright Zero Games. All Rights Reserved.


#include "ZUnrealEngine_Interop.h"

DEFINE_LOG_CATEGORY_STATIC(LogZSharpScript, Log, All)

#define LOG_CASE(Level, Verbosity) case Level: { UE_LOG(LogZSharpScript, Verbosity, TEXT("%s"), message); break; }

void ZSharp::FZUnrealEngine_Interop::Log(uint8 level, const TCHAR* message)
{
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
}

#undef LOG_CASE

uint8 ZSharp::FZUnrealEngine_Interop::IsInGameThread()
{
	return !!::IsInGameThread();
}


