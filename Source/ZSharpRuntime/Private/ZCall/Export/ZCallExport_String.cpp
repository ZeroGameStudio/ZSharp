// Copyright Zero Games. All Rights Reserved.


#include "ZConjugateRegistry.h"
#include "Interop/ZCallBuffer.h"
#include "ZCall/ZStaticExportZCall.h"

namespace ZSharp
{
	static FZStaticExportZCall Export_String_Ctor{ "ex://String.Ctor", [](FZCallBuffer* buffer)
	{
		FZConjugateHandle managed = buffer->Slots[0].Conjugate;
		const TCHAR* data = StaticCast<const TCHAR*>(buffer->Slots[1].Pointer);
		FString* unmanaged = new FString { data };
		FZConjugateRegistry::Get()->Conjugate(unmanaged, managed);
		buffer->Slots[2].Pointer = unmanaged;
		
		return 0;
	}};

	static FZStaticExportZCall Export_String_Dtor{ "ex://String.Dtor", [](FZCallBuffer* buffer)
	{
		FString* string = FZConjugateRegistry::Get()->Conjugate<FString>(buffer->Slots[0].Conjugate);
		FZConjugateRegistry::Get()->ReleaseConjugate(string);
		delete string;
		
		return 0;
	}};
	
	static FZStaticExportZCall Export_String_Len{ "ex://String.Len", [](FZCallBuffer* buffer)
	{
		FString* string = FZConjugateRegistry::Get()->Conjugate<FString>(buffer->Slots[0].Conjugate);
		int32* res = &buffer->Slots[1].Int32;
		*res = string->Len();
		
		return 0;
	}};

	static FZStaticExportZCall Export_String_GetData{ "ex://String.GetData", [](FZCallBuffer* buffer)
	{
		FString* string = FZConjugateRegistry::Get()->Conjugate<FString>(buffer->Slots[0].Conjugate);
		const TCHAR* res = StaticCast<const TCHAR*>(buffer->Slots[1].Pointer);
		const TCHAR** pRes = StaticCast<const TCHAR**>(&res);

		*pRes = **string;
		
		return 0;
	}};

	static FZStaticExportZCall Export_String_SetData{ "ex://String.SetData", [](FZCallBuffer* buffer)
	{
		FString* string = FZConjugateRegistry::Get()->Conjugate<FString>(buffer->Slots[0].Conjugate);
		const TCHAR* data = StaticCast<const TCHAR*>(buffer->Slots[1].Pointer);
		*string = data;
		
		return 0;
	}};
}


