// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class FCustomThunkRegistry
	{
		friend struct FRegisterCustomThunk;

	public:
		static FCustomThunkRegistry& Get();
		
	public:
		FNativeFuncPtr GetThunk(FName name);
	
	private:
		void Register(FName name, FNativeFuncPtr thunk);

	private:
		TMap<FName, FNativeFuncPtr> ThunkMap;
		
	};
}


