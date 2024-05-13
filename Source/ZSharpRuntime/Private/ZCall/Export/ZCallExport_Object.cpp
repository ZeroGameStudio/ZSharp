// Copyright Zero Games. All Rights Reserved.


#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "ZCall/ZCallBuffer.h"
#include "ZCall/ZStaticExportZCall.h"
#include "ZCall/Conjugate/ZConjugateRegistry_String.h"
#include "ZCall/Conjugate/ZConjugateRegistry_UObject.h"

namespace ZSharp
{
	static FZStaticExportZCall Export_Object_GetClass{ "ex://Object.GetClass", [](FZCallBuffer* buffer)
	{
		IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc();
		FZConjugateRegistry_UObject& reg = alc->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		FZCallBuffer& buf = *buffer;
		
		UObject* obj = reg.Conjugate(buf[0].ReadConjugate());
		UObject* cls = obj->GetClass();
		buf[1].WriteConjugate(reg.Conjugate(cls));
		
		return 0;
	}};

	static FZStaticExportZCall Export_Object_GetOuter{ "ex://Object.GetOuter", [](FZCallBuffer* buffer)
	{
		IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc();
		FZConjugateRegistry_UObject& reg = alc->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		FZCallBuffer& buf = *buffer;
		
		UObject* obj = reg.Conjugate(buf[0].ReadConjugate());
		UObject* outer = obj->GetOuter();
		buf[1].WriteConjugate(reg.Conjugate(outer));
		
		return 0;
	}};

	static FZStaticExportZCall Export_Object_GetName{ "ex://Object.GetName", [](FZCallBuffer* buffer)
	{
		IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc();
		FZConjugateRegistry_UObject& oreg = alc->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		FZConjugateRegistry_String& sreg = alc->GetConjugateRegistry<FZConjugateRegistry_String>();
		FZCallBuffer& buf = *buffer;
		
		UObject* obj = oreg.Conjugate(buf[0].ReadConjugate());
		FString* res = new FString { obj->GetName() };
		buf[1].WriteConjugate(sreg.Conjugate(res, true));
		
		return 0;
	}};
}


