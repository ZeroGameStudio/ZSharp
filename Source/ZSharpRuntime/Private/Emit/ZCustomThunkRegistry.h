// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class FZCustomThunkRegistry
	{
		friend struct FZRegisterCustomThunk;

	public:
		static FZCustomThunkRegistry& Get();
		
	public:
		FNativeFuncPtr GetThunk(FName name);
	
	private:
		void Register(FName name, FNativeFuncPtr thunk);

	private:
		TMap<FName, FNativeFuncPtr> ThunkMap;
		
	};
}


