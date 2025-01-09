// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpReplicatedObject.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UZSharpReplicatedObject : public UInterface
{
	GENERATED_BODY()
};

class ZSHARPRUNTIME_API IZSharpReplicatedObject : public IInterface
{
	GENERATED_BODY()

protected:
	void ZSharpReplicatedObject_GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const;
	
};


