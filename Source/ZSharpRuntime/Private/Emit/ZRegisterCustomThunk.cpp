// Copyright Zero Games. All Rights Reserved.

#include "Emit/ZRegisterCustomThunk.h"

#include "ZCustomThunkRegistry.h"

ZSharp::FZRegisterCustomThunk::FZRegisterCustomThunk(FName name, FNativeFuncPtr thunk)
{
	FZCustomThunkRegistry::Get().Register(name, thunk);
}


