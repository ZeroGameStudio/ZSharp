// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp::ExportStrongObject
{
	namespace ZCallExport_StrongObject_Private
	{
		UObject* Get(const FZSelfDescriptiveStrongObjectPtr& wrapper) { return wrapper.Get(); }
		// Here it must be const ref, otherwise it will try to copy out after function call.
		void Set(const FZSelfDescriptiveStrongObjectPtr& wrapper, UObject* obj) { const_cast<FZSelfDescriptiveStrongObjectPtr&>(wrapper).Set(obj); }
	}
	
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_StrongObject_Private::Get, FStrongObject::Get)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_StrongObject_Private::Set, FStrongObject::Set)
}


