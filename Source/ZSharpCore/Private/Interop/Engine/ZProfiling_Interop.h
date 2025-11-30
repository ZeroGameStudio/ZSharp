// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZProfiling_Interop
	{
		static FCpuProfilerTrace::FEventScope* BeginCpuProfilerEvent(const TCHAR* name, const TCHAR* file, int32 line);
		static void EndCpuProfilerEvent(FCpuProfilerTrace::FEventScope* scope);
	};
}


