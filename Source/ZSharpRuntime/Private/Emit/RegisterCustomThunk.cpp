// Copyright Zero Games. All Rights Reserved.


#include "Emit/RegisterCustomThunk.h"

#include "CustomThunkRegistry.h"

ZSharp::FRegisterCustomThunk::FRegisterCustomThunk(FName name, FNativeFuncPtr thunk)
{
	FCustomThunkRegistry::Get().Register(name, thunk);
}


