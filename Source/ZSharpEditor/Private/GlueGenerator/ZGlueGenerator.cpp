// Copyright Zero Games. All Rights Reserved.


#include "ZGlueGenerator.h"

#include "CLR/IZSharpCLR.h"

static FAutoConsoleCommand GCmdDotnetRun(
	TEXT("gluegenerator"),
	TEXT(""),
	FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
	{
		ZSharp::IZGlueGenerator::Get().Generate();
	}),
	ECVF_Default);

void ZSharp::FZGlueGenerator::HandleComplete()
{
	FZGlueGenerator& self = StaticCast<FZGlueGenerator&>(Get());
	FWriteScopeLock _(self.Lock);

	UE_LOG(LogTemp, Log, TEXT("Total Time Used: %fs"), FPlatformTime::Seconds() - self.StartSeconds);
	
	self.bBusy = false;
	self.OnComplete = nullptr;
	self.Processed = 0;
	self.Total = 0;
	self.LastProcessed = 0;
	self.StartSeconds = 0;
	FTSTicker::GetCoreTicker().RemoveTicker(self.TickerDelegate);
	self.TickerDelegate.Reset();

	AsyncTask(ENamedThreads::GameThread, []
	{
		FZGlueGenerator& self = StaticCast<FZGlueGenerator&>(Get());
		FWriteScopeLock _(self.Lock);
		
		if (self.SlowTask)
		{
			self.SlowTask.Reset();
		}
	});
}

void ZSharp::FZGlueGenerator::HandleUpdate(int32 Processed, int32 Total)
{
	FZGlueGenerator& self = StaticCast<FZGlueGenerator&>(Get());

	self.Processed = Processed;
	self.Total = Total;
}

void ZSharp::FZGlueGenerator::Generate(const TFunction<void()>& onComplete)
{
	FWriteScopeLock _(Lock);
	
	if (bBusy)
	{
		return;
	}
	
	if (!IZSharpCLR::Get().RunAsync(FPaths::Combine(FPaths::ProjectPluginsDir(), "ZeroGames", "ZSharp", "Binaries", "Managed", "ZeroGames.ZSharp.GlueGenerator.dll"), &Args))
	{
		OnComplete = onComplete;
		bBusy = true;
		StartSeconds = FPlatformTime::Seconds();

		TickerDelegate = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this](float)
		{
			if (!SlowTask)
			{
				SlowTask = MakeUnique<FScopedSlowTask>(Total);
				SlowTask->MakeDialog();
			}
			
			SlowTask->EnterProgressFrame(Processed - LastProcessed, FText::FromString("Generating glue code..."));
			LastProcessed = Processed;
			return true;
		}));
	}
}

bool ZSharp::FZGlueGenerator::IsBusy()
{
	FReadScopeLock _(Lock);
	
	return bBusy;
}

ZSharp::IZGlueGenerator* ZSharp::FZGlueGenerator::GetRunningTask()
{
	checkNoEntry();
	return nullptr;
}
