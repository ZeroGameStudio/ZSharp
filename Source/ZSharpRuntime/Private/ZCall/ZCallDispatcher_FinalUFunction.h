// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Function/ZFunctionVisitorHandle.h"
#include "ZCall/IZCallDispatcher.h"

namespace ZSharp
{
	class FZCallDispatcher_FinalUFunction : public IZCallDispatcher
	{

	public:
		explicit FZCallDispatcher_FinalUFunction(const FString& name)
			: Name(name)
			, Path(name.RightChop(5)){}
		
	public:
		virtual const FString& GetName() const override { return Name; }
		virtual EZCallErrorCode Dispatch(FZCallBuffer* buffer) const override;

	private:
		bool ResolveFinalFunction() const;
		bool InvalidateCache() const;

	private:
		FString Name;
		FString Path;
		mutable TWeakObjectPtr<UFunction> FinalFunction;
		mutable bool bNativeFinalFunction = false;
		mutable FZFunctionVisitorHandle Function;
		mutable const FZFunctionVisitor* NativeFunction = nullptr;
		
	};
}


