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
		// This is called from ZCallDispatcher_UFunction to support call UFunction from managed side.
		EZCallErrorCode InvokeUFunction(FZCallBuffer* buffer) const;
		// These three is called from SelfDescriptiveDelegates to support execute/broadcast dynamic delegate from managed side.
		EZCallErrorCode InvokeScriptDelegate(FZCallBuffer* buffer) const;
		EZCallErrorCode InvokeMulticastInlineScriptDelegate(FZCallBuffer* buffer) const;
		EZCallErrorCode InvokeMulticastSparseScriptDelegate(FZCallBuffer* buffer) const;
		
	public:
		// This is called from ZSharpFunction to support call managed code via UFunction.
		EZCallErrorCode InvokeZCall(UObject* object, FFrame& stack) const;
		// This is called from ManagedDelegateProxy to support call managed code via dynamic delegate.
		EZCallErrorCode InvokeZCall(UObject* object, void* params) const;

	private:
		static void StaticClearAlcSensitiveStates();
		static FZCallHandle GetDelegateZCallHandle();

	private:
		explicit FZFunctionVisitor(UFunction* function);

	private:
		bool IsAvailable() const { return bAvailable; }

	private:
		static FZCallHandle DelegateZCallHandle;

	private:
		// This is the top-level function which means it isn't an override function and doesn't have super function.
		TWeakObjectPtr<UFunction> Function;

		bool bAvailable;

		bool bIsStatic;
		bool bIsDelegate;
		bool bIsMulticastDelegate;
		bool bIsRpc;
		TArray<TUniquePtr<IZPropertyVisitor>> ParameterProperties;
		TUniquePtr<IZPropertyVisitor> ReturnProperty;
		TSet<int32> OutParamIndices;
		
	};
}


