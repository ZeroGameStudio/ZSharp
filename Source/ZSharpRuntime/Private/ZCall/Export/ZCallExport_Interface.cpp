// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp::ExportInterface
{
	namespace ZCallExport_Interface_Private
	{
		UObject* Get(const FZSelfDescriptiveScriptInterface& wrapper) { return wrapper.Get(); }
		// Here it must be const ref, otherwise it will try to copy out after function call.
		void Set(const FZSelfDescriptiveScriptInterface& wrapper, UObject* obj) { const_cast<FZSelfDescriptiveScriptInterface&>(wrapper).Set(obj); }
	}
	
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Interface_Private::Get, FInterface::Get)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Interface_Private::Set, FInterface::Set)
}


