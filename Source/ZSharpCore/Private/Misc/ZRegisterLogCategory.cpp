// Copyright Zero Games. All Rights Reserved.


#include "Misc/ZRegisterLogCategory.h"

#include "ZLogCategoryRegistry.h"

ZSharp::FZRegisterLogCategory::FZRegisterLogCategory(FName name, FLogCategoryBase& category)
{
	FZLogCategoryRegistry::Get().Register(name, category);
}


