// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IFieldNotificationClassDescriptor.h"

#include "ZSharpFieldNotifyObject.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UZSharpFieldNotifyObject : public UInterface
{
	GENERATED_BODY()
};

class ZSHARPRUNTIME_API IZSharpFieldNotifyObject : public IInterface
{
	GENERATED_BODY()

protected:
	template <typename BaseType, bool BeforeBase = true>
	requires std::is_base_of_v<UE::FieldNotification::IClassDescriptor, BaseType>
	struct TFieldNotificationDescriptor : BaseType
	{
		virtual void ForEachField(const UClass* cls, TFunctionRef<bool(UE::FieldNotification::FFieldId)> callback) const override
		{
			if constexpr (BeforeBase)
			{
				BaseType::ForEachField(cls, callback);
			}

			IZSharpFieldNotifyObject::ForEachField(cls, callback);

			if constexpr (!BeforeBase)
			{
				BaseType::ForEachField(cls, callback);
			}
		}
	};

private:
	static void ForEachField(const UClass* cls, TFunctionRef<bool(UE::FieldNotification::FFieldId)> callback);
	
};


