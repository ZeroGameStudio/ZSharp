﻿// Copyright Zero Games. All Rights Reserved.

#include "ZLog_Interop.h"

#include "Misc/Log/ZLogCategoryRegistry.h"
#include "Misc/Log/ZRegisterLogCategory.h"

DEFINE_LOG_CATEGORY_STATIC(LogZSharpScriptCore, Log, All)
DEFINE_LOG_CATEGORY_STATIC(LogZSharpScriptAssemblyResolver, Log, All)
DEFINE_LOG_CATEGORY_STATIC(LogZSharpScriptAsync, Log, All)
DEFINE_LOG_CATEGORY_STATIC(LogZSharpScriptEngine, Log, All)
DEFINE_LOG_CATEGORY_STATIC(LogZSharpScript, Log, All)

namespace ZSharp::ZLog_Interop_Private
{
	FZRegisterLogCategory RegisterLogZSharpScriptCore { LogZSharpScriptCore };
	FZRegisterLogCategory RegisterLogZSharpScriptAssemblyResolver { LogZSharpScriptAssemblyResolver };
	FZRegisterLogCategory RegisterLogZSharpScriptAsync { LogZSharpScriptAsync };
	FZRegisterLogCategory RegisterLogZSharpScriptEngine { LogZSharpScriptEngine };
	FZRegisterLogCategory RegisterLogZSharpScript { LogZSharpScript };
	FZRegisterLogCategory RegisterLogTemp { LogTemp };
}

void ZSharp::FZLog_Interop::Log(const TCHAR* category, ELogVerbosity::Type verbosity, const TCHAR* message)
{
	const auto name = FName { category };
	FLogCategoryBase* pLogCategory = FZLogCategoryRegistry::Get().GetCategory(name);
	if (!pLogCategory)
	{
		pLogCategory = &LogZSharpScript;
	}
	FLogCategoryBase& logCategory = *pLogCategory;

	switch (verbosity)
	{
#define LOG_CASE(Verbosity) case ELogVerbosity::Verbosity: { UE_LOG_REF(logCategory, Verbosity, TEXT("%s"), message); break; }
		
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
}

