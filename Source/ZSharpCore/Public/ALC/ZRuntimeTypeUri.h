// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct ZSHARPCORE_API FZRuntimeTypeUri
	{
		FZRuntimeTypeUri() = default;
		explicit FZRuntimeTypeUri(const FString& rootKey);
		FZRuntimeTypeUri(const FString& rootKey, const FZRuntimeTypeUri& inner);
		FZRuntimeTypeUri(const FString& rootKey, const FZRuntimeTypeUri& inner, const FZRuntimeTypeUri& outer);

		bool IsValid() const { return !Uri.IsEmpty(); }
		operator bool() const { return IsValid(); }
		FString operator*() const { return Uri;}
		
		FString Uri;
	};
}


