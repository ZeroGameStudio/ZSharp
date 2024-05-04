// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedMember.h"
#include "ZCallBufferSlotType.h"

namespace ZSharp
{
	class IZExportedMethod;
	
	class ZSHARPEXPORT_API IZExportedType : public IZExportedMember
	{
	public:
		virtual const FString& GetAssemblyName() const = 0;
		virtual const FString& GetSubpathName() const = 0;
		virtual const FString& GetBaseTypeName() const = 0;
		virtual const TArray<FString>& GetInterfaceNames() const = 0;
		virtual EZCallBufferSlotType GetSlotType() const = 0;
	public:
		virtual const TArray<IZExportedMethod*>& GetMethods() const = 0;
	};
}


