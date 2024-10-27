// Copyright Zero Games. All Rights Reserved.

#include "Misc/Log/ZRegisterLogCategory.h"

#include "ZLogCategoryRegistry.h"

ZSharp::FZRegisterLogCategory::FZRegisterLogCategory(FLogCategoryBase& category)
{
	FZLogCategoryRegistry::Get().Register(category);
}


