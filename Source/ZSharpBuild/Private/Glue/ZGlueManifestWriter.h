// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "DTO/ZExportedAssemblyDto.h"

namespace ZSharp
{
	class IZExportedType;
	class IZExportedClass;
	class IZExportedEnum;
	
	class FZGlueManifestWriter
	{

	public:
		void Write();

	private:
		void WriteClass(const IZExportedClass& cls);
		void WriteEnum(const IZExportedEnum& enm);

		TUniquePtr<FZExportedAssemblyDto>* GetAssemblyDto(const IZExportedType& type);

	private:
		TMap<FString, TUniquePtr<FZExportedAssemblyDto>> AssemblyDtoMap; // DTO can be extreme large so we save pointer to map.
		
	};
}


