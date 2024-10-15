// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Function/ZFunctionVisitorHandle.h"
#include "ZCall/IZCallDispatcher.h"

namespace ZSharp
{
	class FZCallDispatcher_VirtualUFunction : public IZCallDispatcher
	{

	public:
		explicit FZCallDispatcher_VirtualUFunction(const FString& name);
		
	public:
		virtual const FString& GetName() const override { return Name; }
		virtual EZCallErrorCode Dispatch(FZCallBuffer* buffer) const override;

	private:
		bool InvalidateCache() const;

	private:
		FString Name;
		FString ClassPath;
		FName FunctionName;
		mutable FZFunctionVisitorHandle Function;
		
	};
}


