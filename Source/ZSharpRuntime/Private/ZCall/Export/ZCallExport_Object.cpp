// Copyright Zero Games. All Rights Reserved.


#include "ZConjugateRegistry.h"
#include "ZSharpRuntimeLogChannels.h"
#include "ZCall/ZCallBuffer.h"
#include "ZCall/ZStaticExportZCall.h"

namespace ZSharp
{
	static FZStaticExportZCall Export_Object_GetClass{ "ex://Object.GetClass", [](FZCallBuffer* buffer)
	{
		UObject* obj = FZConjugateRegistry::Get()->Conjugate<UObject>(ReadConjugate(buffer, 0));
		UObject* cls = obj->GetClass();
		WriteConjugate(buffer, 1, FZConjugateRegistry::Get()->Conjugate(cls));
		
		return 0;
	}};

	static FZStaticExportZCall Export_Object_GetOuter{ "ex://Object.GetOuter", [](FZCallBuffer* buffer)
	{
		UObject* obj = FZConjugateRegistry::Get()->Conjugate<UObject>(ReadConjugate(buffer, 0));
		UObject* outer = obj->GetOuter();
		WriteConjugate(buffer, 1, FZConjugateRegistry::Get()->Conjugate(outer));
		
		return 0;
	}};

	static FZStaticExportZCall Export_Object_GetName{ "ex://Object.GetName", [](FZCallBuffer* buffer)
	{
		UObject* obj = FZConjugateRegistry::Get()->Conjugate<UObject>(ReadConjugate(buffer, 0));
		FString* res = new FString { obj->GetName() };
		WriteConjugate(buffer, 1, FZConjugateRegistry::Get()->Conjugate(res, [](FString* unmanaged)
		{
			UE_LOG(LogZSharpRuntime, Log, TEXT("Release UnrealString: [%s] [%p]"), **unmanaged, unmanaged);
			delete unmanaged;
		}));
		
		return 0;
	}};
}


