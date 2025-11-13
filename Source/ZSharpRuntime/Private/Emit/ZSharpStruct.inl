// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp::ZSharpStruct_Private
{
	static void SetupPropertyDefaultsExcludeSuper(const FZSharpStruct* zsstrct, void* instance)
	{
		for (const auto& propertyDefault : zsstrct->PropertyDefaults)
		{
			void* propertyAddr = instance;
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
	
	static void SetupPropertyDefaults(const FZSharpStruct* zsstrct, void* instance)
	{
		if (const UStruct* superStruct = zsstrct->GetStruct()->GetSuperStruct())
		{
			const FZSharpStruct* superZsstruct = nullptr;
			if (auto superScriptStruct = Cast<const UScriptStruct>(superStruct))
			{
				superZsstruct = FZSharpFieldRegistry::Get().GetScriptStruct(superScriptStruct);
			}
			else if (auto superClass = Cast<const UClass>(superStruct))
			{
				superZsstruct = FZSharpFieldRegistry::Get().GetClass(superClass);
			}
			else
			{
				checkNoEntry();
			}
			
			if (superZsstruct)
			{
				SetupPropertyDefaults(superZsstruct, instance);
			}
		}
		
		SetupPropertyDefaultsExcludeSuper(zsstrct, instance);
	}
}


