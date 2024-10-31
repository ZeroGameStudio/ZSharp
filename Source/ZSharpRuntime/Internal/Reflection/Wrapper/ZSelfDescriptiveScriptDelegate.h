// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSelfDescriptiveBase.h"
#include "ZSharpErrorCodes.h"
#include "Interop/ZGCHandle.h"

namespace ZSharp
{
	struct FZCallBuffer;
	struct FZFunctionVisitorHandle;
	
	struct ZSHARPRUNTIME_API FZSelfDescriptiveScriptDelegate : TZSelfDescriptiveBase<FZSelfDescriptiveScriptDelegate, UDelegateFunction, FScriptDelegate>
	{
		ZSHARP_SELF_DESCRIPTIVE_GENERATED_BODY_AUTO_CTOR(FZSelfDescriptiveScriptDelegate)

		FZSelfDescriptiveScriptDelegate(FZSelfDescriptiveScriptDelegate&& other) noexcept;
		
		void BindUFunction(UObject* object, FName name);
		UObject* BindManaged(FZGCHandle delegate);
		void Unbind();

		EZCallErrorCode Execute(FZCallBuffer* buffer);

		UObject* GetObject() const;
		FName GetFunctionName() const;

		bool IsBound() const;
		bool IsBoundToObject(const UObject* object) const;

		FZSelfDescriptiveScriptDelegate& operator=(FZSelfDescriptiveScriptDelegate&& other) noexcept;

	private:
		 TPimplPtr<FZFunctionVisitorHandle> Visitor;
		
	};
}


