// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp::ExportMap
{
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptMap::Clear, FMap::Clear)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptMap::Num, FMap::Num)

	static FZStaticallyExportZCall GAdd { "ex://Map.Add", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FZSelfDescriptiveScriptMap& sdsm = TZCallBufferSlotEncoder<FZSelfDescriptiveScriptMap>::Decode(buf[0]);
		sdsm.TryAdd(buf[1], buf[2]);
		
		return EZCallErrorCode::Succeed;
	}};

	static FZStaticallyExportZCall GRemove { "ex://Map.Remove", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FZSelfDescriptiveScriptMap& sdsm = TZCallBufferSlotEncoder<FZSelfDescriptiveScriptMap>::Decode(buf[0]);
		sdsm.Remove(buf[1]);
		
		return EZCallErrorCode::Succeed;
	}};

	static FZStaticallyExportZCall GContains { "ex://Map.Find", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FZSelfDescriptiveScriptMap& sdsm = TZCallBufferSlotEncoder<FZSelfDescriptiveScriptMap>::Decode(buf[0]);
		buf[-1].WriteBool(sdsm.Find(buf[1], buf[2]));
		
		return EZCallErrorCode::Succeed;
	}};
}


