// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class FZLogCategoryRegistry
	{
		friend struct FZRegisterLogCategory;

	public:
		static FZLogCategoryRegistry& Get();
		
	public:
		FLogCategoryBase* GetCategory(FName name);
	
	private:
		void Register(FLogCategoryBase& category);

	private:
		TMap<FName, FLogCategoryBase*> CategoryMap;
		
	};
}


