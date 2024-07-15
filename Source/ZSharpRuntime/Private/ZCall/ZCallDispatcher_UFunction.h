// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Function/ZFunctionVisitorHandle.h"
#include "ZCall/IZCallDispatcher.h"

namespace ZSharp
{
	class FZCallDispatcher_UFunction : public IZCallDispatcher
	{

	public:
		explicit FZCallDispatcher_UFunction(const FString& name)
			: Name(name)
			, Path(name.RightChop(4)){}
		
	public:
		virtual const FString& GetName() const override { return Name; }
		virtual int32 Dispatch(FZCallBuffer* buffer) const override;

	private:
		bool InvalidateCache() const;

	private:
		FString Name;
		FString Path;
		mutable FZFunctionVisitorHandle Function;
		
	};
}


