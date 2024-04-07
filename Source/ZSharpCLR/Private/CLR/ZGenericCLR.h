// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CLR/IZCLR.h"
#include "Interop/IZMasterAssemblyLoadContext.h"

namespace ZSharp
{
	class IZAssemblyLoadContext;
	
	class FZGenericCLR : public IZCLR
	{

	public:
		virtual void Startup() override;

	public:
		virtual IZMasterAssemblyLoadContext* CreateMasterALC() override;
		virtual IZMasterAssemblyLoadContext* GetMasterALC() override;
		virtual IZSlimAssemblyLoadContext* CreateSlimALC(const FString& name) override;
		virtual IZSlimAssemblyLoadContext* GetSlimALC(const FString& name) override;

	private:
		bool bInitialized = false;
		TUniquePtr<IZMasterAssemblyLoadContext> MasterALC;
		
	};
}


