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
		EZCallErrorCode InvokeUFunction(FZCallBuffer* buffer, const UFunction* finalFunction) const;
		// These three are called from SelfDescriptiveDelegates to support execute/broadcast dynamic delegate from managed side.
		EZCallErrorCode InvokeScriptDelegate(FZCallBuffer* buffer) const;
		EZCallErrorCode InvokeMulticastInlineScriptDelegate(FZCallBuffer* buffer) const;
		EZCallErrorCode InvokeMulticastSparseScriptDelegate(FZCallBuffer* buffer) const;
		
	public:
		// This is called from ZSharpFunction to support call managed code via UFunction.
		EZCallErrorCode InvokeZCall(UObject* context, FFrame& stack, RESULT_DECL) const;
		// This is called from ManagedDelegateProxy to support call managed code via dynamic delegate.
		EZCallErrorCode InvokeZCall(UObject* object, void* params) const;

	private:
		EZCallErrorCode InternalInvokeUFunction(FZCallBuffer* buffer, UObject* object, const UFunction* function) const;

	private:
		static void StaticClearAlcSensitiveStates();
		static FZCallHandle GetDelegateZCallHandle();

	private:
		explicit FZFunctionVisitor(UFunction* function);

	private:
		FORCEINLINE void FillParams(void* params, const FZCallBuffer* buffer) const;
		FORCEINLINE void CopyOut(FZCallBuffer* buffer, void* params) const;

	private:
		bool IsAvailable() const { return bAvailable; }

	private:
		static FZCallHandle DelegateZCallHandle;

	private:
		// This is the top-level function which means it isn't an override function and doesn't have super function.
		// IMPORTANT: User is expected to cache a handle instead of pointer to this
		// and the handle can't find this if the function is invalid.
		// Since this is the parent of the function the handle holds
		// so if the handle successfully finds us, it means:
		// 1. If the handle holds a child, then there is at least one reference to this function.
		// 2. If the handle holds this function itself, then it must be valid.
		// So we assume this function is always valid.
		// But we still like to use a smart pointer instead of a raw pointer
		// because it breaks debugging experience.
		TWeakObjectPtr<UFunction> Function;

		bool bAvailable;

		bool bIsStatic;
		bool bIsDelegate;
		bool bIsMulticastDelegate;
		bool bIsRpc;
		bool bUseOptimizedProcessEvent;
		TArray<TUniquePtr<IZPropertyVisitor>> ParameterProperties;
		TUniquePtr<IZPropertyVisitor> ReturnProperty;
		TSet<int32> InParamIndices;
		TSet<int32> NonConstOutParamIndices;
		
	};
}


