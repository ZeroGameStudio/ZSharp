// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class ZSHARPEXPORT_API IZExportedVariable
	{
	public:
		virtual ~IZExportedVariable(){}
	public:
		virtual FString GetName() const = 0;
		virtual FString GetType() const = 0;
	};
}


