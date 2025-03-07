// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpRuntimeMacros.h"

#if ZSHARP_WITH_MASTER_ALC_RELOAD

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	class FZSharpClassInstanceRegistry : public FNoncopyable
	{

		using ThisClass = FZSharpClassInstanceRegistry;
		
	public:
		static FZSharpClassInstanceRegistry& Get();

	public:
		FZSharpClassInstanceRegistry();
		
	public:
		void Register(UObject* instance);

	private:
		void HandleMasterAlcReloaded(IZMasterAssemblyLoadContext* alc);

	private:
		TSet<FObjectKey> Instances;
		
	};
}

#endif


