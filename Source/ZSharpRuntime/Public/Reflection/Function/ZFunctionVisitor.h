// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Property/IZPropertyVisitor.h"
#include "ZCall/ZCallHandle.h"
#include "ZSharpErrorCodes.h"

namespace ZSharp
{
	struct FZCallBuffer;
	
	class FZFunctionVisitor
	{

		friend class FZFunctionVisitorRegistry;
	
	public:
		EZCallErrorCode InvokeUFunction(FZCallBuffer* buffer) const;
		EZCallErrorCode InvokeScriptDelegate(FZCallBuffer* buffer) const;
		EZCallErrorCode InvokeMulticastInlineScriptDelegate(FZCallBuffer* buffer) const;
		EZCallErrorCode InvokeMulticastSparseScriptDelegate(FZCallBuffer* buffer) const;
		EZCallErrorCode InvokeZCall(FZCallHandle handle, UObject* object, void* params) const;

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


