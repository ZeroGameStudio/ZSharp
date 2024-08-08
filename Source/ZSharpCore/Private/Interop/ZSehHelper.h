// Copyright Zero Games. All Rights Reserved.

#pragma once

#if PLATFORM_SEH_EXCEPTIONS_DISABLED
#define GUARDED_INVOKE(Try, Catch) return Try;
#else
#include "HAL/ExceptionHandling.h"
	
namespace ZSharp::ZSehHelper_Private
{
	inline int32 ReportCrash(Windows::LPEXCEPTION_POINTERS ExceptionInfo)
	{
		const int32 res = ::ReportCrash(ExceptionInfo);
		UE_DEBUG_BREAK();
		return res;
	}
}

#define GUARDED_INVOKE(Try, Catch) __try { return Try; } __except(FPlatformMisc::GetCrashHandlingType() == ECrashHandlingType::Default ? (ZSharp::ZSehHelper_Private::ReportCrash(GetExceptionInformation()), EXCEPTION_CONTINUE_SEARCH) : EXCEPTION_CONTINUE_SEARCH) { return Catch; }
#endif


