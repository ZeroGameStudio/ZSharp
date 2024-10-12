// Copyright Zero Games. All Rights Reserved.


#include "CustomThunkRegistry.h"

FNativeFuncPtr ZSharp::FCustomThunkRegistry::GetFunction(FName name)
{
	return FunctionMap[name];
}

GetterFuncPtr ZSharp::FCustomThunkRegistry::GetGetter(FName name)
{
	return GetterMap[name];
}

SetterFuncPtr ZSharp::FCustomThunkRegistry::GetSetter(FName name)
{
	return SetterMap[name];
}

ZSharp::FCustomThunkRegistry& ZSharp::FCustomThunkRegistry::Get()
{
	static FCustomThunkRegistry GInstance;

	return GInstance;
}

void ZSharp::FCustomThunkRegistry::Register(FName name, FNativeFuncPtr thunk)
{
	check(!FunctionMap.Contains(name));
	FunctionMap.Emplace(name, thunk);
}

void ZSharp::FCustomThunkRegistry::Register(FName name, GetterFuncPtr thunk)
{
	check(!GetterMap.Contains(name));
	GetterMap.Emplace(name, thunk);
}

void ZSharp::FCustomThunkRegistry::Register(FName name, SetterFuncPtr thunk)
{
	check(!SetterMap.Contains(name));
	SetterMap.Emplace(name, thunk);
}


