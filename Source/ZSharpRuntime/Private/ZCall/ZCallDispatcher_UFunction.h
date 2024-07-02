// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Property/IZPropertyVisitor.h"
#include "ZCall/IZCallDispatcher.h"

namespace ZSharp
{
	class FZCallDispatcher_UFunction : public IZCallDispatcher
	{

	public:
		FZCallDispatcher_UFunction(const FString& name)
			: Name(name)
			, Path(name.RightChop(4))
			, bAvailable(false){}
		
	public:
		virtual const FString& GetName() const override { return Name; }
		virtual int32 Dispatch(FZCallBuffer* buffer) const override;

	private:
		bool InvalidateCache() const;

	private:
		FString Name;
		FString Path;
		mutable TWeakObjectPtr<UFunction> Function;

		mutable bool bAvailable;
		mutable TArray<TUniquePtr<IZPropertyVisitor>> ParameterProperties;
		mutable TUniquePtr<IZPropertyVisitor> ReturnProperty;
		mutable TArray<int32> OutParamIndices;
		
	};
}


