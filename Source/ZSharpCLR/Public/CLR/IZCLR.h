// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	class IZSlimAssemblyLoadContext;
	
	class ZSHARPCLR_API IZCLR
	{
	public:
		static IZCLR& Get();
	public:
		virtual ~IZCLR(){}
	public:
		virtual void Startup() = 0;
	public:
		virtual IZMasterAssemblyLoadContext* CreateMasterALC() = 0;
		virtual IZMasterAssemblyLoadContext* GetMasterALC() = 0;
		virtual IZSlimAssemblyLoadContext* CreateSlimALC(const FString& name) = 0;
		virtual IZSlimAssemblyLoadContext* GetSlimALC(const FString& name) = 0;
	};
}



