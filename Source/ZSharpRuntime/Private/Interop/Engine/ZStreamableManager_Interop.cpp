// Copyright Zero Games. All Rights Reserved.

#include "ZStreamableManager_Interop.h"

#include "ALC/ZRedFrameScope.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp::ZStreamingTask_Interop_Private
{
	static FZStreamingTask_Interop::FZStreamingTask* RequestAsyncLoading(FStreamableManager* manager, TArray<FString>& paths, uint8 pushLoadedCount)
	{
		auto task = new FZStreamingTask_Interop::FZStreamingTask;

		auto callback = [task, pushLoadedCount]
		{
			FZRedFrameScope scope;
			{
				if (!!pushLoadedCount)
				{
					FZStreamableManager_Interop::GUpdate(task->Handle->GetOwningManager(), task, FZStreamingTask_Interop::GetLoadedCount(task));
				}
				FZStreamableManager_Interop::GSignalCompletion(task->Handle->GetOwningManager(), task);
			}
		};
	
		TSharedPtr<FStreamableHandle> handle = manager->RequestAsyncLoad(paths, callback);

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
					FZRedFrameScope scope;
					{
						FZStreamableManager_Interop::GUpdate(handle->GetOwningManager(), task, FZStreamingTask_Interop::GetLoadedCount(task));
					}
				}));
			}

			handle->BindCancelDelegate(FStreamableDelegate::CreateLambda(callback));
		}

		return task;
	}
}

FStreamableManager* ZSharp::FZStreamableManager_Interop::GetGlobalStreamableManager()
{
	return &UAssetManager::GetStreamableManager();
}

ZSharp::FZStreamingTask_Interop::FZStreamingTask* ZSharp::FZStreamableManager_Interop::RequestAsyncLoading(FStreamableManager* manager, TArray<FString>& paths, uint8 pushLoadedCount)
{
	TRY
	{
		return ZStreamingTask_Interop_Private::RequestAsyncLoading(manager, paths, pushLoadedCount);
	}
	CATCHR(nullptr)
}


