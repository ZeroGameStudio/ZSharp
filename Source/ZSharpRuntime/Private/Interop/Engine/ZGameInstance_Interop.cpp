// Copyright Zero Games. All Rights Reserved.

#include "ZGameInstance_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Engine/GameEngine.h"

ZSharp::FZConjugateHandle ZSharp::FZGameInstance_Interop::GetPrimaryInstance()
{
	UGameInstance* result = nullptr;
	if (auto gameEngine = Cast<const UGameEngine>(GEngine))
	{
		result = gameEngine->GameInstance;
	}
	else if (auto editorEngine = Cast<const UEditorEngine>(GEngine))
	{
		const FWorldContext* primaryPIEWorld = editorEngine->GetWorldContextFromPIEInstance(0);
		result = primaryPIEWorld ? primaryPIEWorld->OwningGameInstance : nullptr;
	}

	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(result);
}


