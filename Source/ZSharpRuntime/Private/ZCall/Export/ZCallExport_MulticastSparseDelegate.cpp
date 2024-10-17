// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp::ExportMulticastSparseDelegate
{
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastSparseScriptDelegate::AddUFunction, FMulticastSparseDelegate::AddUFunction)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastSparseScriptDelegate::AddManaged, FMulticastSparseDelegate::AddManaged)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastSparseScriptDelegate::RemoveUFunction, FMulticastSparseDelegate::RemoveUFunction)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastSparseScriptDelegate::RemoveAll, FMulticastSparseDelegate::RemoveAll)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastSparseScriptDelegate::Clear, FMulticastSparseDelegate::Clear)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveMulticastSparseScriptDelegate::IsBound, FMulticastSparseDelegate::IsBound)

	static FZStaticallyExportZCall GBroadcast { "ex://MulticastSparseDelegate.Broadcast", [](FZCallBuffer* buffer)
	{
		return TZCallBufferSlotEncoder<FZSelfDescriptiveMulticastSparseScriptDelegate>::Decode((*buffer)[0]).Broadcast(buffer);
	}};
}


