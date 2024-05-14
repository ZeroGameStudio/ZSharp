// Copyright Zero Games. All Rights Reserved.


#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "ZCall/ZStaticExportZCall.h"
#include "ZCall/Conjugate/ZConjugateRegistry_String.h"

namespace ZSharp
{
	static FZStaticExportZCall Export_String_Len{ "ex://String.Len", [](FZCallBuffer* buffer)
	{
		IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc();
		FZConjugateRegistry_String& reg = alc->GetConjugateRegistry<FZConjugateRegistry_String>();
		FZCallBuffer& buf = *buffer;
		
		FString* string = reg.Conjugate(buf[0].ReadConjugate());
		buf[1].WriteInt32(string->Len());
		
		return 0;
	}};

	static FZStaticExportZCall Export_String_GetData{ "ex://String.GetData", [](FZCallBuffer* buffer)
	{
		IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc();
		FZConjugateRegistry_String& reg = alc->GetConjugateRegistry<FZConjugateRegistry_String>();
		FZCallBuffer& buf = *buffer;
		
		FString* string = reg.Conjugate(buf[0].ReadConjugate());
		buf[1].WritePointer(const_cast<TCHAR*>(**string));
		
		return 0;
	}};

	static FZStaticExportZCall Export_String_SetData{ "ex://String.SetData", [](FZCallBuffer* buffer)
	{
		IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc();
		FZConjugateRegistry_String& reg = alc->GetConjugateRegistry<FZConjugateRegistry_String>();
		FZCallBuffer& buf = *buffer;
		
		FString* string = reg.Conjugate(buf[0].ReadConjugate());
		const TCHAR* data = StaticCast<const TCHAR*>(buf[1].ReadPointer());
		*string = data;
		
		return 0;
	}};
}


