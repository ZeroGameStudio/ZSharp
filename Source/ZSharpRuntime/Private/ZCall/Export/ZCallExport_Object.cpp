// Copyright Zero Games. All Rights Reserved.


#include "ZConjugateRegistry.h"
#include "ZSharpRuntimeLogChannels.h"
#include "ZCall/ZCallBuffer.h"
#include "ZCall/ZStaticExportZCall.h"

namespace ZSharp
{
	static FZStaticExportZCall Export_Object_GetClass{ "ex://Object.GetClass", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		UObject* obj = FZConjugateRegistry::Get()->Conjugate<UObject>(buf[0].ReadConjugate());
		UObject* cls = obj->GetClass();
		buf[1].WriteConjugate(FZConjugateRegistry::Get()->Conjugate(cls));
		
		return 0;
	}};

	static FZStaticExportZCall Export_Object_GetOuter{ "ex://Object.GetOuter", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		UObject* obj = FZConjugateRegistry::Get()->Conjugate<UObject>(buf[0].ReadConjugate());
		UObject* outer = obj->GetOuter();
		buf[1].WriteConjugate(FZConjugateRegistry::Get()->Conjugate(outer));
		
		return 0;
	}};

	static FZStaticExportZCall Export_Object_GetName{ "ex://Object.GetName", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		UObject* obj = FZConjugateRegistry::Get()->Conjugate<UObject>(buf[0].ReadConjugate());
		FString* res = new FString { obj->GetName() };
		buf[1].WriteConjugate(FZConjugateRegistry::Get()->Conjugate(res, [](FString* unmanaged)
		{
			UE_LOG(LogZSharpRuntime, Log, TEXT("Release UnrealString: [%s] [%p]"), **unmanaged, unmanaged);
			delete unmanaged;
		}));
		
		return 0;
	}};
}


