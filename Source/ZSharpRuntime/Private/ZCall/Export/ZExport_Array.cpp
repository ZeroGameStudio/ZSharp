// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticExportZCall.h"
#include "ZCall/ZStaticExportZCallMacros.h"
#include "ZCall/Conjugate/ZConjugateRegistry_Array.h"

namespace ZSharp
{
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptArray::InsertAt, _Array::InsertAt)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptArray::RemoveAt, _Array::RemoveAt)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptArray::Num, _Array::Num)

	static FZStaticExportZCall GGet { "ex://Array.Get", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FZSelfDescriptiveScriptArray& sdsa = TZCallBufferSlotEncoder<FZSelfDescriptiveScriptArray>::Decode(buf[0]);
		int32 index = buf[1].ReadInt32();
		sdsa.Get(index, buf[2]);
		
		return 0;
	}};

	static FZStaticExportZCall GSet { "ex://Array.Set", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FZSelfDescriptiveScriptArray& sdsa = TZCallBufferSlotEncoder<FZSelfDescriptiveScriptArray>::Decode(buf[0]);
		int32 index = buf[1].ReadInt32();
		sdsa.Set(index, buf[2]);
		
		return 0;
	}};
}


