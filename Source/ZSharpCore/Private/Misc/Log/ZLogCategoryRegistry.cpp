// Copyright Zero Games. All Rights Reserved.

#include "ZLogCategoryRegistry.h"

ZSharp::FZLogCategoryRegistry& ZSharp::FZLogCategoryRegistry::Get()
{
	static FZLogCategoryRegistry GInstance;

	return GInstance;
}

FLogCategoryBase* ZSharp::FZLogCategoryRegistry::GetCategory(FName name)
{
	const auto category = CategoryMap.Find(name);
	return category ? *category : nullptr;
}

void ZSharp::FZLogCategoryRegistry::Register(FLogCategoryBase& category)
{
	FName name = category.GetCategoryName();
	check(!CategoryMap.Contains(name));
	CategoryMap.Emplace(name, &category);
}


