// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp
{
	namespace ZCallExport_WeakObject_Private
	{
		UObject* Get(const FZSelfDescriptiveWeakObjectPtr& wrapper) { return wrapper.Get(); }
		// Here it must be const ref, otherwise it will try to copy out after function call.
		void Set(const FZSelfDescriptiveWeakObjectPtr& wrapper, UObject* obj) { const_cast<FZSelfDescriptiveWeakObjectPtr&>(wrapper).Set(obj); }
		bool IsNull(const FZSelfDescriptiveWeakObjectPtr& wrapper) { return wrapper.GetUnderlyingInstance()->IsExplicitlyNull(); }
	}
	
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_WeakObject_Private::Get, FWeakObject::Get)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_WeakObject_Private::Set, FWeakObject::Set)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_WeakObject_Private::IsNull, FWeakObject::IsNull)
}


