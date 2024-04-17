// Copyright Zero Games. All Rights Reserved.


#include "ZConjugateRegistry.h"
#include "Interop/ZCallBuffer.h"
#include "ZCall/ZStaticExportZCall.h"

namespace ZSharp
{
	static FZStaticExportZCall Export_Object_GetClass{ "ex://Object.GetClass", [](FZCallBuffer* buffer)
	{
		UObject* obj = FZConjugateRegistry::Get().Conjugate<UObject>(buffer->Slots[0].Conjugate);
		FZConjugateHandle* res = &buffer->Slots[1].Conjugate;
		UObject* cls = obj->GetClass();
		*res = FZConjugateRegistry::Get().BuildConjugate(cls);
		
		return 0;
	}};

	static FZStaticExportZCall Export_Object_GetOuter{ "ex://Object.GetOuter", [](FZCallBuffer* buffer)
	{
		UObject* obj = FZConjugateRegistry::Get().Conjugate<UObject>(buffer->Slots[0].Conjugate);
		FZConjugateHandle* res = &buffer->Slots[1].Conjugate;
		UObject* outer = obj->GetOuter();
		*res = FZConjugateRegistry::Get().BuildConjugate(outer);
		
		return 0;
	}};

	static FZStaticExportZCall Export_Object_GetName{ "ex://Object.GetName", [](FZCallBuffer* buffer)
	{
		UObject* obj = FZConjugateRegistry::Get().Conjugate<UObject>(buffer->Slots[0].Conjugate);
		FZConjugateHandle* res = &buffer->Slots[1].Conjugate;
		FString* name = new FString{ obj->GetName() };
		*res = FZConjugateRegistry::Get().BuildConjugate(name);
		
		return 0;
	}};
}


