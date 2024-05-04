// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedMember.h"

namespace ZSharp
{
	class IZExportedParameter;

	class ZSHARPEXPORT_API IZExportedMethod : public IZExportedMember
	{
	public:
		virtual const FString& GetZCallName() const = 0;
	public:
		virtual const IZExportedParameter* GetReturnParameter() const = 0;
		virtual const TArray<IZExportedParameter*>& GetParameters() const = 0;
	};
}


