// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSelfDescriptiveBase.h"
#include "Interop/ZGCHandle.h"
#include "Reflection/Function/ZFunctionVisitorHandle.h"

namespace ZSharp
{
	struct FZCallBuffer;
	
	struct ZSHARPRUNTIME_API FZSelfDescriptiveScriptDelegate : TZSelfDescriptiveBase<FZSelfDescriptiveScriptDelegate, UFunction, FScriptDelegate>
	{
		ZSHARP_SELF_DESCRIPTIVE_GENERATED_BODY_AUTO_CTOR(FZSelfDescriptiveScriptDelegate)
		
		void BindUFunction(UObject* object, FName name);
		UObject* BindManaged(FZGCHandle delegate);
		void Unbind();

		int32 Execute(FZCallBuffer* buffer);

		UObject* GetObject() const;
		FName GetFunctionName() const;
		const UFunction* GetFunction() const;
		FString GetZCallName() const;

		bool IsBound() const;
		bool IsBoundToObject(const UObject* object) const;

	private:
		 FZFunctionVisitorHandle Visitor;
		
	};
}


