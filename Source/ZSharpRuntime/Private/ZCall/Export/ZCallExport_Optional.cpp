// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp::ExportOptional
{
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveOptional::Reset, FOptional::Reset)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveOptional::IsSet, FOptional::IsSet)

	static FZStaticallyExportZCall GGet { "ex://Optional.Get", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FZSelfDescriptiveOptional& sdo = TZCallBufferSlotEncoder<FZSelfDescriptiveOptional>::Decode(buf[0]);
		buf[-1].WriteBool(sdo.Get(buf[1]));
		
		return EZCallErrorCode::Succeed;
	}};

	static FZStaticallyExportZCall GSet { "ex://Optional.Set", [](FZCallBuffer* buffer)
	{
		FZCallBuffer& buf = *buffer;
		FZSelfDescriptiveOptional& sdo = TZCallBufferSlotEncoder<FZSelfDescriptiveOptional>::Decode(buf[0]);
		sdo.Set(buf[1]);
		
		return EZCallErrorCode::Succeed;
	}};
}


