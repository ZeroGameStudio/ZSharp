// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZUnrealFieldEmitter_Interop
	{
		static void InternalConstructScriptStructInstance(void* instance, const UScriptStruct* scriptStruct);
		static void InternalDestructScriptStructInstance(void* instance, const UScriptStruct* scriptStruct);
		static void InternalReloadStructOpsFakeVTable(const TCHAR* scriptStruct, void* constructThunk, void* destructThunk);
	};
}


