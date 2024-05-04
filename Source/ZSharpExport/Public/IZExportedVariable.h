// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class ZSHARPEXPORT_API IZExportedVariable
	{
	public:
		virtual ~IZExportedVariable(){}
	public:
		virtual const FString& GetName() const = 0;
		virtual const FString& GetTypeName() const = 0;
	};
}


