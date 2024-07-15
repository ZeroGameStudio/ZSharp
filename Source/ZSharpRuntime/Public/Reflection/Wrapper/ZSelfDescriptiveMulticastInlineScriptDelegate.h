// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSelfDescriptiveBase.h"
#include "Interop/ZGCHandle.h"
#include "Reflection/Function/ZFunctionVisitorHandle.h"

namespace ZSharp
{
	struct FZCallBuffer;

	struct ZSHARPRUNTIME_API FZSelfDescriptiveMulticastScriptDelegate : TZSelfDescriptiveBase<FZSelfDescriptiveMulticastScriptDelegate, UFunction, FMulticastScriptDelegate>
	{
		ZSHARP_SELF_DESCRIPTIVE_GENERATED_BODY_AUTO_CTOR(FZSelfDescriptiveMulticastScriptDelegate)
		
		void AddUFunction(UObject* object, FName name);
		UObject* AddManaged(FZGCHandle delegate);
		void RemoveUFunction(const UObject* object, FName name);
		void RemoveAll(const UObject* object);
		void Clear();

		int32 Broadcast(FZCallBuffer* buffer);

		bool IsBound() const;

	private:
		FZFunctionVisitorHandle Visitor;
		
	};
}


