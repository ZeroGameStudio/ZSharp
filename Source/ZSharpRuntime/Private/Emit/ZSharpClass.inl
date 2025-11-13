// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpClassInstanceRegistry.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRedFrameScope.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "ZCall/ZCallBuffer.h"

#include "ZSharpStruct.inl"
#include "Net/Core/PushModel/PushModel.h"

namespace ZSharp::ZSharpClass_Private
{
	static void ConstructObject(const FObjectInitializer& objectInitializer, UClass* cls, IZMasterAssemblyLoadContext* alc, FZConjugateHandle conjugate, bool& contextual)
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
			
			ConstructObject(objectInitializer, cls->GetSuperClass(), alc, conjugate, contextual);
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

			ZSharpStruct_Private::SetupPropertyDefaultsExcludeSuper(zscls, obj);
			if (zscls->bDefaultToReplicated)
			{
				static constexpr uint8 GMaxByteAsTrue = 0xFF;
				if (obj->IsA<AActor>())
				{
					static const FProperty* GActorReplicatesProperty = AActor::StaticClass()->FindPropertyByName("bReplicates");
					GActorReplicatesProperty->SetValue_InContainer(obj, &GMaxByteAsTrue);
				}
				else if (obj->IsA<UActorComponent>())
				{
					static const FProperty* GComponentReplicatesProperty = UActorComponent::StaticClass()->FindPropertyByName("bReplicates");
					GComponentReplicatesProperty->SetValue_InContainer(obj, &GMaxByteAsTrue);
					MARK_PROPERTY_DIRTY(obj, GComponentReplicatesProperty);
				}
				else
				{
					ensure(false);
				}
			}

			{ // Call managed red constructor and UClass constructor.
				contextual |= zscls->bContextual;
				
				if (zscls->bConstructor)
				{
					checkNoEntry();
					
					FZCallHandle handle = zscls->GetConstructorZCallHandle();
					FZRedFrameScope scope;
					{
						FZCallBufferSlot slot { EZCallBufferSlotType::Conjugate, { .Conjugate = conjugate } };
						FZCallBuffer buffer;
						buffer.Slots = &slot;
						buffer.NumSlots = 1;
						
						alc->ZCall(handle, &buffer);
					}
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
		IZMasterAssemblyLoadContext* alc = nullptr; // IZSharpClr::Get().GetMasterAlc();
		FZConjugateHandle conjugate = {}; // alc->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(objectInitializer.GetObj());
		bool contextual = false;
		ConstructObject(objectInitializer, objectInitializer.GetClass(), alc, conjugate, contextual);
		// @TODO: Construct on async loading thread.
		// const_cast<FObjectInitializer&>(objectInitializer).AddPropertyPostInitCallback([alc, conjugate]
		// {
		// 	FZCallHandle handle = FZSharpFieldRegistry::Get().GetObjectPostInitPropertiesZCallHandle();
		// 	FZRedFrameScope scope;
		// 	{
		// 		FZCallBufferSlot slot { EZCallBufferSlotType::Conjugate, { .Conjugate = conjugate } };
		// 		FZCallBuffer buffer;
		// 		buffer.Slots = &slot;
		// 		buffer.NumSlots = 1;
		// 							
		// 		alc->ZCall(handle, &buffer);
		// 	}
		// });

#if ZSHARP_WITH_MASTER_ALC_RELOAD
		if (contextual)
		{
			FZSharpClassInstanceRegistry::Get().Register(objectInitializer.GetObj());
		}
#endif
	}
}


