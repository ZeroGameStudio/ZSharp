// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class FZUEnumUnderlyingTypeLookup final
	{

	public:
		static FZUEnumUnderlyingTypeLookup& Get();

	public:
		void InvalidateCache();
		
	public:
		FString GetEnumUnderlyingTypeName(const UEnum* enm) const;

	private:
		TMap<const UEnum*, FString> Cache;
		
	};
}


