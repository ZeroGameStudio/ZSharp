// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp
{
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptArray::InsertAt, FArray::InsertAt)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptArray::RemoveAt, FArray::RemoveAt)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptArray::Num, FArray::Num)

	static FZStaticallyExportZCall GGet { "ex://Array.Get", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FZSelfDescriptiveScriptArray& sdsa = TZCallBufferSlotEncoder<FZSelfDescriptiveScriptArray>::Decode(buf[0]);
		int32 index = buf[1].ReadInt32();
		sdsa.Get(index, buf[2]);
		
		return EZCallErrorCode::Succeed;
	}};

	static FZStaticallyExportZCall GSet { "ex://Array.Set", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FZSelfDescriptiveScriptArray& sdsa = TZCallBufferSlotEncoder<FZSelfDescriptiveScriptArray>::Decode(buf[0]);
		int32 index = buf[1].ReadInt32();
		sdsa.Set(index, buf[2]);
		
		return EZCallErrorCode::Succeed;
	}};
}


