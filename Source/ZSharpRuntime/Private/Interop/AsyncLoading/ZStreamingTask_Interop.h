// Copyright Zero Games. All Rights Reserved.

#pragma once

struct FStreamableHandle;

namespace ZSharp
{
	struct FZConjugateHandle;
	
	struct FZStreamingTask_Interop
	{
		struct FZStreamingTask
		{
			TSharedPtr<FStreamableHandle> Handle;
		};
		
		static int32 GetResult(FZStreamingTask* task, FZConjugateHandle* buffer, int32 size);
		static int32 GetLoadedCount(FZStreamingTask* task);
		static void Release(FZStreamingTask* task);
	};
}


