// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp
{
	namespace ZCallExport_Interface_Private
	{
		UObject* Get(const FZSelfDescriptiveScriptInterface& interface) { return interface.Get(); }
		// Here it must be const ref, otherwise it will try to copy out after function call.
		void Set(const FZSelfDescriptiveScriptInterface& interface, UObject* obj) { const_cast<FZSelfDescriptiveScriptInterface&>(interface).Set(obj); }
	}
	
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Interface_Private::Get, FInterface::Get)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_Interface_Private::Set, FInterface::Set)
}


