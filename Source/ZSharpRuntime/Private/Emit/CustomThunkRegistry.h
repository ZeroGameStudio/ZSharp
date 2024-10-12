// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class FCustomThunkRegistry
	{
		friend class FRegisterCustomThunk;
		
	public:
		FNativeFuncPtr GetFunction(FName name);
		GetterFuncPtr GetGetter(FName name);
		SetterFuncPtr GetSetter(FName name);
		
	private:
		static FCustomThunkRegistry& Get();

	private:
		void Register(FName name, FNativeFuncPtr thunk);
		void Register(FName name, GetterFuncPtr thunk);
		void Register(FName name, SetterFuncPtr thunk);

	private:
		TMap<FName, FNativeFuncPtr> FunctionMap;
		TMap<FName, GetterFuncPtr> GetterMap;
		TMap<FName, SetterFuncPtr> SetterMap;
		
	};
}


