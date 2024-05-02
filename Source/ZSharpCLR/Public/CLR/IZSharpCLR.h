// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	class IZSlimAssemblyLoadContext;

	DECLARE_MULTICAST_DELEGATE_OneParam(FZOnMasterAlcLoaded, IZMasterAssemblyLoadContext*)
	DECLARE_MULTICAST_DELEGATE(FZOnMasterAlcUnloaded)
	
	class ZSHARPCLR_API IZSharpCLR
	{
	public:
		static IZSharpCLR& Get();
	public:
		virtual ~IZSharpCLR(){}
	public:
		virtual void CollectGarbage(int32 generation = -1, bool bAggressive = true, bool bBlocking = false, bool bCompacting = true) = 0;
	public:
		virtual IZMasterAssemblyLoadContext* CreateMasterAlc() = 0;
		virtual IZMasterAssemblyLoadContext* GetMasterAlc() = 0;
		virtual void GetMasterAlc_AnyThread(TFunctionRef<void(IZMasterAssemblyLoadContext*)> action) = 0;
		virtual IZSlimAssemblyLoadContext* CreateSlimAlc(const FString& name) = 0;
		virtual IZSlimAssemblyLoadContext* GetSlimAlc(const FString& name) = 0;
	public:
		virtual FDelegateHandle RegisterMasterAlcLoaded(FZOnMasterAlcLoaded::FDelegate delegate, bool bNotifyIfLoaded = true) = 0;
		virtual void UnregisterMasterAlcLoaded(FDelegateHandle delegate) = 0;
		virtual void UnregisterMasterAlcLoaded(const void* userObject) = 0;
		virtual FDelegateHandle RegisterMasterAlcUnloaded(FZOnMasterAlcUnloaded::FDelegate delegate) = 0;
		virtual void UnregisterMasterAlcUnloaded(FDelegateHandle delegate) = 0;
		virtual void UnregisterMasterAlcUnloaded(const void* userObject) = 0;
		// Helpers
	public:
		int32 Run(const FString& path, void* args = nullptr, const FString& alcName = "");
		int32 RunAsync(const FString& path, void* args = nullptr, const FString& alcName = "");
	};
}



