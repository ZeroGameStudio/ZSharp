﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSelfDescriptiveBase.h"
#include "ZSharpErrorCodes.h"
#include "Interop/ZGCHandle.h"

namespace ZSharp
{
	struct FZCallBuffer;
	struct FZFunctionVisitorHandle;

	struct ZSHARPRUNTIME_API FZSelfDescriptiveMulticastInlineScriptDelegate : TZSelfDescriptiveBase<FZSelfDescriptiveMulticastInlineScriptDelegate, UDelegateFunction, FMulticastScriptDelegate>
	{
		ZSHARP_SELF_DESCRIPTIVE_GENERATED_BODY_AUTO_CTOR(FZSelfDescriptiveMulticastInlineScriptDelegate)

		FZSelfDescriptiveMulticastInlineScriptDelegate(FZSelfDescriptiveMulticastInlineScriptDelegate&& other) noexcept;
		
		void AddUFunction(UObject* object, FName name);
		UObject* AddManaged(FZGCHandle delegate);
		UObject* AddManaged(FZGCHandle delegate, FZGCHandle state);
		void RemoveUFunction(const UObject* object, FName name);
		void RemoveAll(const UObject* object);
		void Clear();

		EZCallErrorCode Broadcast(FZCallBuffer* buffer);

		bool IsBound() const;
		bool IsBoundToObject(const UObject* object) const;
		bool Contains(const UObject* object, FName name) const;

		FZSelfDescriptiveMulticastInlineScriptDelegate& operator=(FZSelfDescriptiveMulticastInlineScriptDelegate&& other) noexcept;

	private:
		TPimplPtr<FZFunctionVisitorHandle> Visitor;
		
	};
}


