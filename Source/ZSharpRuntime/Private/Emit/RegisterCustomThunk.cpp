// Copyright Zero Games. All Rights Reserved.


#include "Emit/RegisterCustomThunk.h"

#include "CustomThunkRegistry.h"

ZSharp::FRegisterCustomThunk::FRegisterCustomThunk(FName name, FNativeFuncPtr thunk)
{
	FCustomThunkRegistry::Get().Register(name, thunk);
}

ZSharp::FRegisterCustomThunk::FRegisterCustomThunk(FName name, GetterFuncPtr thunk)
{
	FCustomThunkRegistry::Get().Register(name, thunk);
}

ZSharp::FRegisterCustomThunk::FRegisterCustomThunk(FName name, SetterFuncPtr thunk)
{
	FCustomThunkRegistry::Get().Register(name, thunk);
}


