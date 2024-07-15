// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp
{
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastInlineScriptDelegate::AddUFunction, FMulticastInlineDelegate::AddUFunction)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastInlineScriptDelegate::AddManaged, FMulticastInlineDelegate::AddManaged)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastInlineScriptDelegate::RemoveUFunction, FMulticastInlineDelegate::RemoveUFunction)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastInlineScriptDelegate::RemoveAll, FMulticastInlineDelegate::RemoveAll)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastInlineScriptDelegate::Clear, FMulticastInlineDelegate::Clear)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastInlineScriptDelegate::IsBound, FMulticastInlineDelegate::IsBound)

	static FZStaticallyExportZCall GBroadcast { "ex://MulticastInlineDelegate.Broadcast", [](FZCallBuffer* buffer)
	{
		return TZCallBufferSlotEncoder<FZSelfDescriptiveMulticastInlineScriptDelegate>::Decode((*buffer)[0]).Broadcast(buffer);
	}};
}


