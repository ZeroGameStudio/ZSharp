// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp::ExportSet
{
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptSet::Clear, FSet::Clear)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptSet::Num, FSet::Num)

	static FZStaticallyExportZCall GAdd { "ex://Set.Add", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FZSelfDescriptiveScriptSet& sdss = TZCallBufferSlotEncoder<FZSelfDescriptiveScriptSet>::Decode(buf[0]);
		sdss.Add(buf[1]);
		
		return EZCallErrorCode::Succeed;
	}};

	static FZStaticallyExportZCall GRemove { "ex://Set.Remove", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FZSelfDescriptiveScriptSet& sdss = TZCallBufferSlotEncoder<FZSelfDescriptiveScriptSet>::Decode(buf[0]);
		sdss.Remove(buf[1]);
		
		return EZCallErrorCode::Succeed;
	}};

	static FZStaticallyExportZCall GContains { "ex://Set.Contains", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FZSelfDescriptiveScriptSet& sdss = TZCallBufferSlotEncoder<FZSelfDescriptiveScriptSet>::Decode(buf[0]);
		buf[-1].WriteBool(sdss.Contains(buf[1]));
		
		return EZCallErrorCode::Succeed;
	}};
}


