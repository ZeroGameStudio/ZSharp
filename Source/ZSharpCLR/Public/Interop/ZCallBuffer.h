// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateHandle.h"

namespace ZSharp
{
	union FZCallBufferSlot
	{
		uint8 UInt8;
		uint16 UInt16;
		uint32 UInt32;
		uint64 UInt64;
		int8 Int8;
		int16 Int16;
		int32 Int32;
		int64 Int64;
		float Float;
		double Double;
		uint8 Bool;
		void* Pointer;
		FZConjugateHandle Conjugate;
	};
	
	struct ZSHARPCLR_API FZCallBuffer
	{
		FZCallBufferSlot* Slots;
	};
}
