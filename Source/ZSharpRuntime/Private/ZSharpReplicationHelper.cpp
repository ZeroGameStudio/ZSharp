// Copyright Zero Games. All Rights Reserved.


#include "ZSharpReplicationHelper.h"

#include "Emit/IZSharpFieldRegistry.h"

void ZSharp::FZSharpReplicationHelper::GetZSharpLifetimeReplicatedProps(const UObject* object, TArray<FLifetimeProperty>& outLifetimeProps)
{
	bool found = false;
	const UClass* currentClass = object->GetClass();
	while (currentClass)
	{
		const FZSharpClass* zscls = IZSharpFieldRegistry::Get().GetClass(currentClass);
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


