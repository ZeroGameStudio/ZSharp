// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Property/IZPropertyVisitor.h"
#include "ZCall/ZCallHandle.h"

namespace ZSharp
{
	struct FZCallBuffer;
	
	class FZFunctionVisitor
	{

		friend class FZFunctionVisitorRegistry;
	
	public:
		int32 InvokeUFunction(FZCallBuffer* buffer) const;
		int32 InvokeScriptDelegate(FZCallBuffer* buffer) const;
		int32 InvokeMulticastInlineScriptDelegate(FZCallBuffer* buffer) const;
		int32 InvokeMulticastSparseScriptDelegate(FZCallBuffer* buffer) const;
		int32 InvokeZCall(FZCallHandle handle, UObject* object, void* params) const;

	private:
		explicit FZFunctionVisitor(UFunction* function);

	private:
		bool IsAvailable() const { return bAvailable; }

	private:
		TWeakObjectPtr<UFunction> Function;

		bool bAvailable;

		bool bIsStatic;
		bool bIsDelegate;
		bool bIsMulticastDelegate;
		bool bIsRpc;
		TArray<TUniquePtr<IZPropertyVisitor>> ParameterProperties;
		TUniquePtr<IZPropertyVisitor> ReturnProperty;
		TArray<int32> OutParamIndices;
		
	};
}


