// Copyright Zero Games. All Rights Reserved.

#include "ZSharpReplicatedObject.h"

#include "Emit/IZSharpFieldRegistry.h"

void IZSharpReplicatedObject::ZSharpReplicatedObject_GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const
{
	const UObject* object = _getUObject();
	const UClass* currentClass = object->GetClass();
	while (currentClass)
	{
		const ZSharp::FZSharpClass* zscls = ZSharp::IZSharpFieldRegistry::Get().GetClass(currentClass);
		if (zscls)
		{
			for (const auto& replicatedProperty : zscls->ReplicatedProperties)
			{
				outLifetimeProps.AddUnique(FLifetimeProperty { replicatedProperty.Property->RepIndex, replicatedProperty.Condition, replicatedProperty.RepNotifyCondition, replicatedProperty.bPushBased });
			}
		}
		else if (currentClass->IsNative())
		{
			break;
		}

		currentClass = currentClass->GetSuperClass();
	}
}


