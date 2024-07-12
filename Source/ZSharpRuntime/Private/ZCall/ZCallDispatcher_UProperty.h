// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Property/IZPropertyVisitor.h"
#include "ZCall/IZCallDispatcher.h"

namespace ZSharp
{
	class FZCallDispatcher_UProperty : public IZCallDispatcher
	{

	public:
		explicit FZCallDispatcher_UProperty(const FString& name);
		
	public:
		virtual const FString& GetName() const override { return Name; }
		virtual int32 Dispatch(FZCallBuffer* buffer) const override;

	private:
		bool InvalidateCache() const;

	private:
		FString Name;
		FString StructPath;
		FString PropertyName;
		mutable TWeakObjectPtr<UStruct> GCRoot; // FProperty's lifetime is not managed by GC, but follows it's outer.
		mutable TUniquePtr<IZPropertyVisitor> Property;

		mutable bool bAvailable;
		
	};
}


