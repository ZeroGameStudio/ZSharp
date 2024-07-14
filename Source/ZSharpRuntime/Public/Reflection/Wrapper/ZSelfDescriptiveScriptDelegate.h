// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSelfDescriptiveBase.h"
#include "Interop/ZGCHandle.h"

namespace ZSharp
{
	struct ZSHARPRUNTIME_API FZSelfDescriptiveScriptDelegate : TZSelfDescriptiveBase<FZSelfDescriptiveScriptDelegate, UFunction, FScriptDelegate>
	{
		ZSHARP_SELF_DESCRIPTIVE_GENERATED_BODY_AUTO_CTOR(FZSelfDescriptiveScriptDelegate)
		
		void BindUFunction(UObject* object, FName name);
		UObject* BindManaged(FZGCHandle delegate);
		void Unbind();

		void Execute(void* params);

		UObject* GetObject() const;
		FName GetFunctionName() const;
		const UFunction* GetFunction() const;
		FString GetZCallName() const;

		bool IsBound() const;
		bool IsBoundToObject(const UObject* object) const;
		
	};
}


