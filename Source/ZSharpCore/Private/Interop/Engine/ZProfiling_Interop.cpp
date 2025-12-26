// Copyright Zero Games. All Rights Reserved.

#include "ZProfiling_Interop.h"

#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp::ZProfiling_Interop_Private
{
	static FCpuProfilerTrace::FEventScope* BeginCpuProfilerEvent(const TCHAR* name, const TCHAR* file, int32 line)
	{
		static TMap<FName, uint32> GEventIdMap;
	
		FName id = FName { FString::Printf(TEXT("%s%s%d"), name, file, line) };
		FAnsiString ansiName = name;
		FAnsiString ansiFile = file;
		
		return new FCpuProfilerTrace::FEventScope { GEventIdMap.FindOrAdd(id), *ansiName, true, *ansiFile, static_cast<uint32>(line) };
	}
}

FCpuProfilerTrace::FEventScope* ZSharp::FZProfiling_Interop::BeginCpuProfilerEvent(const TCHAR* name, const TCHAR* file, int32 line)
{
	TRY
	{
		return ZProfiling_Interop_Private::BeginCpuProfilerEvent(name, file, line);
	}
	CATCHR(nullptr)
}

void ZSharp::FZProfiling_Interop::EndCpuProfilerEvent(FCpuProfilerTrace::FEventScope* scope)
{
	GUARD(delete scope);
}


