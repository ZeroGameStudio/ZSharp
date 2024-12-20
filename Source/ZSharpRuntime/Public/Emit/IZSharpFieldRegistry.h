﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZCall/ZCallHandle.h"

namespace ZSharp
{
	struct FZSharpClass
	{
		struct FPropertyDefault
		{
			TArray<FProperty*> PropertyChain;
			FString Buffer;
		};

		struct FDefaultSubobject
		{
			FName Name;
			FName ClassPath;
			bool bOptional = false;
			bool bTransient = false;

			FName PropertyName;
			FObjectPropertyBase* Property = nullptr;
			
			bool bRootComponent = false;
			FName AttachParentDefaultSubobjectName;
			FName AttachSocketName;
		};

		struct FDefaultSubobjectOverride
		{
			FName Name;
			FName ClassPath;
		};

		struct FReplicatedProperty
		{
			FProperty* Property;
			ELifetimeCondition Condition;
			ELifetimeRepNotifyCondition RepNotifyCondition;
			bool bPushBased;
		};

		const UClass* Class = nullptr;
		
		TArray<FPropertyDefault> PropertyDefaults;
		TArray<FDefaultSubobject> DefaultSubobjects;
		TArray<FDefaultSubobjectOverride> DefaultSubobjectOverrides;

		TArray<UE::FieldNotification::FFieldId> FieldNotifies;
		TArray<FReplicatedProperty> ReplicatedProperties;
	};

	struct FZSharpFunction
	{

		friend class FZSharpFieldRegistry;

	public:
		FZCallHandle GetZCallHandle() const;
		FZCallHandle GetValidateZCallHandle() const;

	public:
		const UFunction* Function = nullptr;
		
		FString ZCallName;
		FString ValidateZCallName;
		
	private:
		mutable FZCallHandle ZCallHandle{};
		mutable FZCallHandle ValidateZCallHandle{};
		
	};

	class ZSHARPRUNTIME_API IZSharpFieldRegistry
	{
	public:
		static IZSharpFieldRegistry& Get();
	public:
		virtual ~IZSharpFieldRegistry(){}
	public:
		virtual const FZSharpClass* GetClass(const UClass* cls) const = 0;
		virtual const FZSharpFunction* GetFunction(const UFunction* function) const = 0;
	public:
		bool IsZSharpClass(const UClass* cls) const { return !!GetClass(cls); }
		bool IsZSharpFunction(const UFunction* function) const { return !!GetFunction(function); }
	};
}


