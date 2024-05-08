// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "DTO/ZExportedAssemblyDto.h"

namespace ZSharp
{
	class IZExportedEnum;
	
	class FZGlueManifestWriter
	{

	public:
		void Write();

	private:
		TMap<FString, TUniquePtr<FZExportedAssemblyDto>> AssemblyDtoMap; // DTO can be extreme large so we save pointer to map.
		
	};
}


