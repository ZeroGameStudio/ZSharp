// Copyright Zero Games. All Rights Reserved.

#pragma once

#ifndef ZSHARP_DISABLE_INTEROP_GUARD
#define ZSHARP_DISABLE_INTEROP_GUARD 1
#endif

#if PLATFORM_SEH_EXCEPTIONS_DISABLED || ZSHARP_DISABLE_INTEROP_GUARD
#define TRY if constexpr (true)
#define CATCH if constexpr (false)
#define CATCHR(Return)
#define GUARD(Code) Code;
#else
#include "HAL/ExceptionHandling.h"

namespace ZSharp::ZInteropExceptionHelper_Private
{
	inline int32 ReportCrash(Windows::LPEXCEPTION_POINTERS ExceptionInfo)
	{
		const int32 res = ::ReportCrash(ExceptionInfo);
		UE_DEBUG_BREAK();
		return res;
	}
}

#define TRY __try
#define CATCH __except(FPlatformMisc::GetCrashHandlingType() == ECrashHandlingType::Default ? (ZSharp::ZInteropExceptionHelper_Private::ReportCrash(GetExceptionInformation()), EXCEPTION_CONTINUE_SEARCH) : EXCEPTION_CONTINUE_SEARCH)
#define CATCHR(Return) CATCH { return Return; }
#define GUARD(Code) TRY { Code; } CATCH{}

#endif


