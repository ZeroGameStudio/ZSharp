// Copyright Zero Games. All Rights Reserved.

#include "ZStreamingTask_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Engine/StreamableManager.h"

int32 ZSharp::FZStreamingTask_Interop::GetResult(FZStreamingTask* task, FZConjugateHandle* buffer, int32 size)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	int32 writeCount = 0;
	task->Handle->ForEachLoadedAsset([buffer, size, &registry, &writeCount](const UObject* asset)
	{
		if (!ensure(writeCount < size))
		{
			return;
		}

		buffer[writeCount++] = registry.Conjugate(asset);
	});

	return writeCount;
}

int32 ZSharp::FZStreamingTask_Interop::GetLoadedCount(FZStreamingTask* task)
{
	int32 loadedCount;
	int32 dummy;
	task->Handle->GetLoadedCount(loadedCount, dummy);
	return loadedCount;
}

void ZSharp::FZStreamingTask_Interop::Release(FZStreamingTask* task)
{
	delete task;
}


