// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class ZSHARPEXPORT_API IZExportedMember
	{
	public:
		virtual ~IZExportedMember(){}
	public:
		virtual FString GetName() const = 0;
	};
}


