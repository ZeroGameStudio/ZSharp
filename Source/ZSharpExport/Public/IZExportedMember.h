// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class ZSHARPEXPORT_API IZExportedMember
	{
	public:
		virtual ~IZExportedMember(){}
	public:
		virtual const FString& GetName() const = 0;
		virtual bool IsStatic() const = 0;
	};
}


