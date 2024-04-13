// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZCallBuffer;
	
	class ZSHARPCLR_API IZCallDispatcher
	{
	public:
		virtual ~IZCallDispatcher(){}
	public:
		virtual const FString& GetName() const = 0;
		virtual int32 Dispatch(FZCallBuffer* buffer) const = 0;
	};
}


