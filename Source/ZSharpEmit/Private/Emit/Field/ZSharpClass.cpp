// Copyright Zero Games. All Rights Reserved.


#include "Emit/Field/ZSharpClass.h"

namespace ZSharp::ZSharpClass_Private
{
	static void ConstructObject(const FObjectInitializer& objectInitializer, UClass* cls)
	{
		UObject* obj = objectInitializer.GetObj();

		if (cls->GetClass() != UClass::StaticClass())
		{
			// If we are not C++ class (Z# or BPGC) then call super constructor first.
			ConstructObject(objectInitializer, cls->GetSuperClass());

			// Setup Z# properties if we are Z# class.
			if (const auto zscls = Cast<UZSharpClass>(cls))
			{
				for (TFieldIterator<FProperty> it(zscls, EFieldIteratorFlags::ExcludeSuper); it; ++it)
				{
					// @TODO: Default value.
					it->InitializeValue_InContainer(obj);
				}
			}
		}
		else
		{
			// Only call self constructor and stop call up because C++ itself will handle the rest.
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
	ClassConstructor = ZSharp::ZSharpClass_Private::ClassConstructor;
}


