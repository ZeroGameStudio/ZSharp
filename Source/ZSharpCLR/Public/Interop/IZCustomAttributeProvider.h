// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class IZAttribute;
	
	class ZSHARPCLR_API IZCustomAttributeProvider
	{
	public:
		virtual ~IZCustomAttributeProvider(){}
	public:
		virtual IZAttribute* GetCustomAttribute(const FString& name) const = 0;
	};
}