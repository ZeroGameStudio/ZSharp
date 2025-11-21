// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealFieldEmitter_Interop.h"

#include "Emit/ZUnrealFieldEmitter.h"

void ZSharp::FZUnrealFieldEmitter_Interop::InternalConstructScriptStructInstance(void* instance, const UScriptStruct* scriptStruct)
{
	FZUnrealFieldEmitterHelper::ConstructScriptStructInstance(instance, scriptStruct);
}

void ZSharp::FZUnrealFieldEmitter_Interop::InternalDestructScriptStructInstance(void* instance, const UScriptStruct* scriptStruct)
{
	FZUnrealFieldEmitterHelper::DestructScriptStructInstance(instance, scriptStruct);
}

void ZSharp::FZUnrealFieldEmitter_Interop::InternalReloadStructOpsFakeVTable(const TCHAR* scriptStruct, void* constructThunk, void* destructThunk)
{
	FZUnrealFieldEmitterHelper::ReloadStructOpsFakeVTable(scriptStruct, constructThunk, destructThunk);
}


