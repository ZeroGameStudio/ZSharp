// Copyright Zero Games. All Rights Reserved.


#include "ZCallBufferSlotType.h"

FString ZSharp::ToString(EZCallBufferSlotType type)
{
	static const TMap<EZCallBufferSlotType, FString> GMap =
	{
#define Op(Type) { EZCallBufferSlotType::Type, #Type },
		ZSHARP_FOREACH_ZCALL_BUFFER_SLOT_TYPE()
#undef Op
	};

	const FString* res = GMap.Find(type);
	return res ? *res : "Unknown";
}


