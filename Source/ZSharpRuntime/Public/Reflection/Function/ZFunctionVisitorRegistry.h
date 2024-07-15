// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZFunctionVisitor.h"
#include "ZFunctionVisitorHandle.h"

namespace ZSharp
{
	class FZFunctionVisitorRegistry: public FNoncopyable
	{

		friend FZFunctionVisitorHandle;

	public:
		static const FZFunctionVisitorRegistry& Get();

	public:
		FZFunctionVisitorHandle Get(const UFunction* function) const;

	private:
		const FZFunctionVisitor* Get(FZFunctionVisitorHandle handle) const;

	private:
		mutable TMap<FZFunctionVisitorHandle, TUniquePtr<FZFunctionVisitor>> FunctionMap;
		
	};
}


