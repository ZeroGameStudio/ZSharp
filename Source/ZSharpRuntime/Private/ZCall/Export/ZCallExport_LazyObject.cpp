// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp::ExportLazyObject
{
	namespace ZCallExport_LazyObject_Private
	{
		UObject* Get(const FZSelfDescriptiveLazyObjectPtr& wrapper) { return wrapper.Get(); }
		// Here it must be const ref, otherwise it will try to copy out after function call.
		void Set(const FZSelfDescriptiveLazyObjectPtr& wrapper, UObject* obj) { const_cast<FZSelfDescriptiveLazyObjectPtr&>(wrapper).Set(obj); }
		bool IsNull(const FZSelfDescriptiveLazyObjectPtr& wrapper) { return wrapper.GetUnderlyingInstance()->IsNull(); }
	}
	
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_LazyObject_Private::Get, FLazyObject::Get)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_LazyObject_Private::Set, FLazyObject::Set)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_LazyObject_Private::IsNull, FLazyObject::IsNull)
}


