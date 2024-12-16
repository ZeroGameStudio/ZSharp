// Copyright Zero Games. All Rights Reserved.

#include "ZStreamableManager_Interop.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

FStreamableManager* ZSharp::FZStreamableManager_Interop::GetGlobalStreamableManager()
{
	return &UAssetManager::GetStreamableManager();
}

ZSharp::FZStreamingTask_Interop::FZStreamingTask* ZSharp::FZStreamableManager_Interop::RequestAsyncLoading(FStreamableManager* manager, TArray<FString>& paths, uint8 pushLoadedCount)
{
	auto task = new FZStreamingTask_Interop::FZStreamingTask;
	TSharedPtr<FStreamableHandle> handle = manager->RequestAsyncLoad(paths, [task]
	{
		GSignalCompletion(task->Handle->GetOwningManager(), task);
	});

	if (!handle.IsValid())
	{
		delete task;
		return nullptr;
	}

	task->Handle = handle;
	
	if (handle->IsLoadingInProgress())
	{
		if (!!pushLoadedCount)
		{
			handle->BindUpdateDelegate(FStreamableUpdateDelegate::CreateLambda([task](TSharedPtr<FStreamableHandle> handle)
			{
				GUpdate(handle->GetOwningManager(), task, FZStreamingTask_Interop::GetLoadedCount(task));
			}));
		}

		handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([task]
		{
			GSignalCompletion(task->Handle->GetOwningManager(), task);
		}));
	}

	return task;
}


