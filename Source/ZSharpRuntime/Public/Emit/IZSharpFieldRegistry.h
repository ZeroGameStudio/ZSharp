// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZCall/ZCallHandle.h"

namespace ZSharp
{
	struct ZSHARPRUNTIME_API FZSharpEnum
	{
		const UEnum* Enum = nullptr;
	};

	struct ZSHARPRUNTIME_API FZSharpStruct
	{
		struct ZSHARPRUNTIME_API FPropertyDefault
		{
			TArray<FProperty*> PropertyChain;
			FString Buffer;
		};

		TArray<FPropertyDefault> PropertyDefaults;
	};

	struct ZSHARPRUNTIME_API FZSharpScriptStruct : public FZSharpStruct
	{
		const UScriptStruct* ScriptStruct = nullptr;

		// The engine will destruct this. (@see: GetDeferredCppStructOps())
		UScriptStruct::ICppStructOps* CppStructOps;
	};

	struct ZSHARPRUNTIME_API FZSharpDelegate
	{
		const UDelegateFunction* Delegate = nullptr;
	};
	
	struct ZSHARPRUNTIME_API FZSharpClass : public FZSharpStruct
	{

		friend class FZSharpFieldRegistry;

		struct ZSHARPRUNTIME_API FDefaultSubobject
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

		struct ZSHARPRUNTIME_API FDefaultSubobjectOverride
		{
			FName Name;
			FName ClassPath;
		};

		struct ZSHARPRUNTIME_API FReplicatedProperty
		{
			FProperty* Property;
			ELifetimeCondition Condition;
			ELifetimeRepNotifyCondition RepNotifyCondition;
			bool bPushBased;
		};

	public:
		FZCallHandle GetConstructorZCallHandle() const;
		
	public:
		const UClass* Class = nullptr;

		bool bConstructor = false;
		bool bContextual = false;
		
		TArray<FDefaultSubobject> DefaultSubobjects;
		TArray<FDefaultSubobjectOverride> DefaultSubobjectOverrides;

		TArray<UE::FieldNotification::FFieldId> FieldNotifies;
		TArray<FReplicatedProperty> ReplicatedProperties;

		FString ConstructorZCallName;

	private:
		mutable FZCallHandle ConstructorZCallHandle{};
		
	};

	struct ZSHARPRUNTIME_API FZSharpFunction
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
		virtual const FZSharpEnum* GetEnum(const UEnum* enm) const = 0;
		virtual const FZSharpScriptStruct* GetScriptStruct(const UScriptStruct* scriptStruct) const = 0;
		virtual const FZSharpDelegate* GetDelegate(const UDelegateFunction* delegate) const = 0;
		virtual const FZSharpClass* GetClass(const UClass* cls) const = 0;
		virtual const FZSharpFunction* GetFunction(const UFunction* function) const = 0;
	public:
		bool IsZSharpEnum(const UEnum* enm) const { return !!GetEnum(enm); }
		bool IsZSharpScriptStruct(const UScriptStruct* scriptStruct) const { return !!GetScriptStruct(scriptStruct); }
		bool IsZSharpDelegate(const UDelegateFunction* delegate) const { return !!GetDelegate(delegate); }
		bool IsZSharpClass(const UClass* cls) const { return !!GetClass(cls); }
		bool IsZSharpFunction(const UFunction* function) const { return !!GetFunction(function); }
	};
}


