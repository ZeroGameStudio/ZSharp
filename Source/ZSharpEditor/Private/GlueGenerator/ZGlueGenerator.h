// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "GlueGenerator/IZGlueGenerator.h"

namespace ZSharp
{
	class FZGlueGenerator : public IZGlueGenerator
	{

	public:
		virtual void Generate(const TFunction<void()>& onComplete) override;
		virtual bool IsBusy() override;
		virtual IZGlueGenerator* GetRunningTask() override;

	private:
		static void HandleComplete();
		static void HandleUpdate(int32 Processed, int32 Total);

	private:
		struct FArgs
		{
			void(*OnComplete)() = HandleComplete;
			void(*OnUpdate)(int32, int32) = HandleUpdate;
		} Args;
		
		FRWLock Lock;
		bool bBusy = false;
		TFunction<void()> OnComplete;
		TAtomic<int32> Processed = 0;
		TAtomic<int32> Total = 0;
		int32 LastProcessed = 0;
		double StartSeconds = 0;
		FTSTicker::FDelegateHandle TickerDelegate;
		TUniquePtr<FScopedSlowTask> SlowTask;
		
	};
}
