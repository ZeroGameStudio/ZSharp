// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/IZCallDispatcher.h"

namespace ZSharp
{
	class FZCallDispatcher_Export : public IZCallDispatcher
	{

	public:
		FZCallDispatcher_Export(const FString& name, const TFunction<int32(FZCallBuffer*)> function)
			: Name(name)
			, Function(function){}

	public:
		virtual const FString& GetName() const override { return Name; }
		virtual int32 Dispatch(FZCallBuffer* buffer) const override { return Function(buffer); }

	private:
		FString Name;
		TFunction<int32(FZCallBuffer*)> Function;
		
	};
}


