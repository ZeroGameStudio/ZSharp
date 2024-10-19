// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct ZSHARPCORE_API FZRegisterLogCategory
	{
		FZRegisterLogCategory(FName name, FLogCategoryBase& category);
	};
}

#define ZSHARP_REGISTER_LOG_CATEGORY(Category) ZSharp::FZRegisterLogCategory __GRegisterLogCategory_##Category { #Category, Category };


