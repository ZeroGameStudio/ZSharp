﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSelfDescriptiveBase.h"
#include "ZSharpErrorCodes.h"
#include "Interop/ZGCHandle.h"
#include "Reflection/Function/ZFunctionVisitorHandle.h"

namespace ZSharp
{
	struct FZCallBuffer;

	struct ZSHARPRUNTIME_API FZSelfDescriptiveMulticastSparseScriptDelegate : TZSelfDescriptiveBase<FZSelfDescriptiveMulticastSparseScriptDelegate, USparseDelegateFunction, FSparseDelegate>
	{
		ZSHARP_SELF_DESCRIPTIVE_GENERATED_BODY_AUTO_CTOR(FZSelfDescriptiveMulticastSparseScriptDelegate)

		FZSelfDescriptiveMulticastSparseScriptDelegate(FZSelfDescriptiveMulticastSparseScriptDelegate&& other) noexcept;
		
		void AddUFunction(UObject* object, FName name);
		UObject* AddManaged(FZGCHandle delegate);
		void RemoveUFunction(const UObject* object, FName name);
		void RemoveAll(const UObject* object);
		void Clear();

		EZCallErrorCode Broadcast(FZCallBuffer* buffer);

		bool IsBound() const;

		FMulticastScriptDelegate* GetDelegatePtr();
		
		FZSelfDescriptiveMulticastSparseScriptDelegate& operator=(FZSelfDescriptiveMulticastSparseScriptDelegate&& other) noexcept;

	private:
		void Add(const FScriptDelegate& unicast);
		const UObject* GetOwner() const;

	private:
		FZFunctionVisitorHandle Visitor;
		
	};
}


