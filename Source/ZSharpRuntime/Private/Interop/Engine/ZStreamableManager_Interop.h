// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZStreamingTask_Interop.h"

struct FStreamableManager;

namespace ZSharp
{
	struct FZStreamableManager_Interop
	{
		static FStreamableManager* GetGlobalStreamableManager();
		static FZStreamingTask_Interop::FZStreamingTask* RequestAsyncLoading(FStreamableManager* manager, TArray<FString>& paths, uint8 pushLoadedCount);

		inline static void(*GUpdate)(FStreamableManager*, FZStreamingTask_Interop::FZStreamingTask*, int32) = nullptr;
		inline static void(*GSignalCompletion)(FStreamableManager*, FZStreamingTask_Interop::FZStreamingTask*) = nullptr;
	};
}


