// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZCallDispatcher.h"

namespace ZSharp
{
	class FZCallDispatcher_UFunction : public IZCallDispatcher
	{

	public:
		FZCallDispatcher_UFunction(const FString& name)
			: Name(name)
			, Path(name.RightChop(4)){}
		
	public:
		virtual const FString& GetName() const override { return Name; }
		virtual int32 Dispatch(FZCallBuffer* buffer) const override;

	private:
		FString Name;
		FString Path;
		mutable TWeakObjectPtr<UFunction> Function;
		
	};
}


