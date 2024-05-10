// Copyright Zero Games. All Rights Reserved.


#include "ZConjugateRegistry.h"
#include "ZCall/ZCallBuffer.h"
#include "ZCall/ZStaticExportZCall.h"

namespace ZSharp
{
	static FZStaticExportZCall Export_String_Ctor{ "ex://String.Ctor", [](FZCallBuffer* buffer)
	{
		FZConjugateHandle managed = ReadConjugate(buffer, 0);
		const TCHAR* data = StaticCast<const TCHAR*>(ReadPointer(buffer, 1));
		FString* unmanaged = new FString { data };
		FZConjugateRegistry::Get()->Conjugate(unmanaged, managed);
		WritePointer(buffer, 2, unmanaged);
		
		return 0;
	}};

	static FZStaticExportZCall Export_String_Dtor{ "ex://String.Dtor", [](FZCallBuffer* buffer)
	{
		FString* string = FZConjugateRegistry::Get()->Conjugate<FString>(ReadConjugate(buffer, 0));
		FZConjugateRegistry::Get()->ReleaseConjugate(string);

		return 0;
	}};
	
	static FZStaticExportZCall Export_String_Len{ "ex://String.Len", [](FZCallBuffer* buffer)
	{
		FString* string = FZConjugateRegistry::Get()->Conjugate<FString>(ReadConjugate(buffer, 0));
		WriteInt32(buffer, 1, string->Len());
		
		return 0;
	}};

	static FZStaticExportZCall Export_String_GetData{ "ex://String.GetData", [](FZCallBuffer* buffer)
	{
		FString* string = FZConjugateRegistry::Get()->Conjugate<FString>(ReadConjugate(buffer, 0));
		WritePointer(buffer, 1, const_cast<TCHAR*>(**string));
		
		return 0;
	}};

	static FZStaticExportZCall Export_String_SetData{ "ex://String.SetData", [](FZCallBuffer* buffer)
	{
		FString* string = FZConjugateRegistry::Get()->Conjugate<FString>(ReadConjugate(buffer, 0));
		const TCHAR* data = StaticCast<const TCHAR*>(ReadPointer(buffer, 1));
		*string = data;
		
		return 0;
	}};
}


