// Copyright Zero Games. All Rights Reserved.

#include "ZSharpReplicatedObject.h"

#include "Emit/IZSharpFieldRegistry.h"

void IZSharpReplicatedObject::ZSharpReplicatedObject_GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const
{
	const UObject* object = _getUObject();
	bool found = false;
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
			found = true;
		}
		else if (found)
		{
			break;
		}

		currentClass = currentClass->GetSuperClass();
	}
}


