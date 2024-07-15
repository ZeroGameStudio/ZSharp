// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp
{
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastScriptDelegate::AddUFunction, FMulticastDelegate::AddUFunction)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastScriptDelegate::AddManaged, FMulticastDelegate::AddManaged)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastScriptDelegate::RemoveUFunction, FMulticastDelegate::RemoveUFunction)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastScriptDelegate::RemoveAll, FMulticastDelegate::RemoveAll)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastScriptDelegate::Clear, FMulticastDelegate::Clear)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastScriptDelegate::IsBound, FMulticastDelegate::IsBound)

	static FZStaticallyExportZCall GBroadcast { "ex://MulticastDelegate.Broadcast", [](FZCallBuffer* buffer)
	{
		return TZCallBufferSlotEncoder<FZSelfDescriptiveMulticastScriptDelegate>::Decode((*buffer)[0]).Broadcast(buffer);
	}};
}


