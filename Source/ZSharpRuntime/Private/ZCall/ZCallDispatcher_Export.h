// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZCall/IZCallDispatcher.h"

namespace ZSharp
{
	class FZCallDispatcher_Export : public IZCallDispatcher
	{

	public:
		FZCallDispatcher_Export(const FString& name, const TFunction<EZCallErrorCode(FZCallBuffer*)>& function)
			: Name(name)
			, Function(function){}

	public:
		virtual const FString& GetName() const override { return Name; }
		virtual EZCallErrorCode Dispatch(FZCallBuffer* buffer) const override { return Function(buffer); }

	private:
		FString Name;
		TFunction<EZCallErrorCode(FZCallBuffer*)> Function;
		
	};
}


