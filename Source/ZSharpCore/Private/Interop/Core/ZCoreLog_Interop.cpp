// Copyright Zero Games. All Rights Reserved.

#include "ZCoreLog_Interop.h"

DEFINE_LOG_CATEGORY_STATIC(LogZSharpScriptCore, Log, All)

void ZSharp::FZCoreLog_Interop::Log(uint8 verbosity, const TCHAR* message)
{
#if !NO_LOGGING
	static const FString GEmpty;
	
	if (!message)
	{
		message = *GEmpty;
	}
	
	switch (static_cast<ELogVerbosity::Type>(verbosity))
	{
#define LOG_CASE(Verbosity) case ELogVerbosity::Verbosity: { UE_LOG(LogZSharpScriptCore, Verbosity, TEXT("%s"), message); break; }
		
		LOG_CASE(Fatal)
		LOG_CASE(Error)
		LOG_CASE(Warning)
		LOG_CASE(Display)
		LOG_CASE(Log)
		LOG_CASE(Verbose)
		LOG_CASE(VeryVerbose)
		
#undef LOG_CASE
		default:
			{
				break;
			}
	}
#endif
}


