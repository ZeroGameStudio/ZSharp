// Copyright Zero Games. All Rights Reserved.


#include "ZConjugateRegistry.h"
#include "ZCall/ZCallBuffer.h"
#include "ZCall/ZStaticExportZCall.h"

namespace ZSharp
{
	static FZStaticExportZCall Export_String_Ctor{ "ex://String.Ctor", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FZConjugateHandle managed = buf[0].ReadConjugate();
		const TCHAR* data = StaticCast<const TCHAR*>(buf[1].ReadPointer());
		FString* unmanaged = new FString { data };
		FZConjugateRegistry::Get()->Conjugate(unmanaged, managed);
		buf[2].WritePointer(unmanaged);
		
		return 0;
	}};

	static FZStaticExportZCall Export_String_Dtor{ "ex://String.Dtor", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FString* string = FZConjugateRegistry::Get()->Conjugate<FString>(buf[0].ReadConjugate());
		FZConjugateRegistry::Get()->ReleaseConjugate(string);

		return 0;
	}};
	
	static FZStaticExportZCall Export_String_Len{ "ex://String.Len", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FString* string = FZConjugateRegistry::Get()->Conjugate<FString>(buf[0].ReadConjugate());
		buf[1].WriteInt32(string->Len());
		
		return 0;
	}};

	static FZStaticExportZCall Export_String_GetData{ "ex://String.GetData", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FString* string = FZConjugateRegistry::Get()->Conjugate<FString>(buf[0].ReadConjugate());
		buf[1].WritePointer(const_cast<TCHAR*>(**string));
		
		return 0;
	}};

	static FZStaticExportZCall Export_String_SetData{ "ex://String.SetData", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FString* string = FZConjugateRegistry::Get()->Conjugate<FString>(buf[0].ReadConjugate());
		const TCHAR* data = StaticCast<const TCHAR*>(buf[1].ReadPointer());
		*string = data;
		
		return 0;
	}};
}


