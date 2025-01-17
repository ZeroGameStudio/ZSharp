// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp::ZSharpClass_Private
{
	static void ConstructObject(const FObjectInitializer& objectInitializer, UClass* cls)
	{
		UObject* obj = objectInitializer.GetObj();

		const FZSharpClass* zscls = FZSharpFieldRegistry::Get().GetClass(cls);
		const bool native = cls->IsNative();
		// If we are dynamic class (Z# class or BPGC) then call super constructor first.
		if (zscls || !native)
		{
			// If we are Z# class, apply default subobject overrides before call super.
			if (zscls)
			{
				for (const auto& defaultSubobjectOverride : zscls->DefaultSubobjectOverrides)
				{
					if (defaultSubobjectOverride.ClassPath.IsNone())
					{
						objectInitializer.DoNotCreateDefaultSubobject(defaultSubobjectOverride.Name);
					}
					else if (auto overrideClass = LoadClass<const UObject>(nullptr, *defaultSubobjectOverride.ClassPath.ToString()))
					{
						objectInitializer.SetDefaultSubobjectClass(defaultSubobjectOverride.Name, overrideClass);
					}
					else
					{
						UE_LOG(LogZSharpEmit, Warning, TEXT("Default subobject override class [%s] not found! Outer: [%s] SubobjectName: [%s]"), *defaultSubobjectOverride.ClassPath.ToString(), *GetNameSafe(obj), *defaultSubobjectOverride.Name.ToString());
					}
				}
			}
			
			ConstructObject(objectInitializer, cls->GetSuperClass());
		}

		// BPGC doesn't need to do anything else.
		if (!native)
		{
			return;
		}
		
		// Setup Z# properties for Z# class.
		if (zscls)
		{
			{ // Initialize Z# memory.
				for (TFieldIterator<FProperty> it(cls, EFieldIteratorFlags::ExcludeSuper); it; ++it)
				{
					it->InitializeValue_InContainer(obj);
				}
			}

			{ // Setup default subobjects.
				for (const auto& defaultSubobject : zscls->DefaultSubobjects)
				{
					if (auto defaultSubobjectClass = LoadClass<const UObject>(nullptr, *defaultSubobject.ClassPath.ToString()))
					{
						// Subclass may not allow us to create so subobject can be null.
						if (UObject* subobject = obj->CreateDefaultSubobject(defaultSubobject.Name, defaultSubobjectClass, defaultSubobjectClass, !defaultSubobject.bOptional, defaultSubobject.bTransient))
						{
							// Associate the subobject with a property.
							if (defaultSubobject.Property)
							{
								defaultSubobject.Property->SetObjectPropertyValue_InContainer(obj, subobject);
							}

							// Setup scene info.
							if (defaultSubobject.bRootComponent)
							{
								auto actor = Cast<AActor>(obj);
								auto root = Cast<USceneComponent>(subobject);
								if (ensureAlways(actor) && ensureAlways(root))
								{
									USceneComponent* prevRoot = actor->GetRootComponent();
									actor->SetRootComponent(root);
									if (prevRoot)
									{
										prevRoot->SetupAttachment(root);
									}
								}
							}
							else if (!defaultSubobject.AttachParentDefaultSubobjectName.IsNone())
							{
								ensureAlways(!defaultSubobject.bRootComponent);

								auto child = Cast<USceneComponent>(subobject);
								auto parent = Cast<USceneComponent>(obj->GetDefaultSubobjectByName(defaultSubobject.AttachParentDefaultSubobjectName));
								if (ensureAlways(child) && ensureAlways(parent))
								{
									child->SetupAttachment(parent, defaultSubobject.AttachSocketName);
								}
							}
						}
					}
					else
					{
						UE_LOG(LogZSharpEmit, Warning, TEXT("Default subobject class [%s] not found! Outer: [%s] SubobjectName: [%s]"), *defaultSubobject.ClassPath.ToString(), *GetNameSafe(obj), *defaultSubobject.Name.ToString());
					}
				}
			}

			{ // Setup property defaults
				for (const auto& propertyDefault : zscls->PropertyDefaults)
				{
					void* propertyAddr = obj;
					FProperty* tailProperty = propertyDefault.PropertyChain[propertyDefault.PropertyChain.Num() - 1];
					// Follow the container chain to resolve the actual container.
					for (const auto& prop : propertyDefault.PropertyChain)
					{
						if (prop->IsA<FStructProperty>() || prop == tailProperty)
						{
							propertyAddr = prop->ContainerPtrToValuePtr<void>(propertyAddr);
						}
						else if (auto objectProp = CastField<const FObjectPropertyBase>(prop))
						{
							propertyAddr = objectProp->GetObjectPropertyValue_InContainer(propertyAddr);
						}
						else
						{
							checkNoEntry();
						}
					}
					
					ensure(tailProperty->ImportText_Direct(*propertyDefault.Buffer, propertyAddr, nullptr, PPF_None));
				}
			}
		}
		// Only call self constructor and stop call up because C++ itself will handle the rest.
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


