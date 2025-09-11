// Copyright Zero Games. All Rights Reserved.

#include "ZSharpFieldNotifyObject.h"

#include "Emit/IZSharpFieldRegistry.h"

void IZSharpFieldNotifyObject::ForEachField(const UClass* cls, TFunctionRef<bool(UE::FieldNotification::FFieldId)> callback)
{
	const UClass* currentClass = cls;
	while (currentClass)
	{
		const ZSharp::FZSharpClass* zscls = ZSharp::IZSharpFieldRegistry::Get().GetClass(currentClass);
		if (zscls)
		{
			for (const auto& fieldId : zscls->FieldNotifies)
			{
				if (!callback(fieldId))
				{
					return;
				}
			}
		}
		else if (currentClass->IsNative())
		{
			break;
		}

		currentClass = currentClass->GetSuperClass();
	}
}


