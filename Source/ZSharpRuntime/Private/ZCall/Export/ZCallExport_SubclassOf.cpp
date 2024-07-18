// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp
{
	namespace ZCallExport_SubclassOf_Private
	{
		UObject* Get(const FZSelfDescriptiveSubclassOf& wrapper) { return wrapper.Get(); }
		// Here it must be const ref, otherwise it will try to copy out after function call.
		void Set(const FZSelfDescriptiveSubclassOf& wrapper, UClass* cls) { const_cast<FZSelfDescriptiveSubclassOf&>(wrapper).Set(cls); }
	}
	
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_SubclassOf_Private::Get, FSubclassOf::Get)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_SubclassOf_Private::Set, FSubclassOf::Set)
}


