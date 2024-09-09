// Copyright Zero Games. All Rights Reserved.


#include "Emit/Field/ZSharpClass.h"

namespace ZSharpClass_Private
{
	static void ConstructObject(const FObjectInitializer& objectInitializer, UClass* cls)
	{
		UObject* obj = objectInitializer.GetObj();
		
		// Construct super first.
		// Only call constructor of the most derived C++ class because C++ constructor will continue to call super.
		if (cls->UObject::IsA<UZSharpClass>())
		{
			ConstructObject(objectInitializer, cls->GetSuperClass());
		}

		// Then construct self.
		if (const auto zscls = Cast<UZSharpClass>(cls))
		{
			for (TFieldIterator<FProperty> it(zscls, EFieldIteratorFlags::ExcludeSuper); it; ++it)
			{
				// @TODO: Default value.
				it->InitializeValue_InContainer(obj);
			}
		}
		else
		{
			cls->ClassConstructor(objectInitializer);
		}
	}
	
	static void ClassConstructor(const FObjectInitializer& objectInitializer)
	{
		ConstructObject(objectInitializer, objectInitializer.GetClass());
	}
}

UZSharpClass::UZSharpClass()
{
	ClassConstructor = ZSharpClass_Private::ClassConstructor;
}


