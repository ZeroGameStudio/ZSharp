// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp
{
	namespace ZCallExport_SoftObject_Private
	{
		UObject* Get(const FZSelfDescriptiveSoftObjectPtr& wrapper) { return wrapper.Get(); }
		// Here it must be const ref, otherwise it will try to copy out after function call.
		void Set(const FZSelfDescriptiveSoftObjectPtr& wrapper, UObject* obj) { const_cast<FZSelfDescriptiveSoftObjectPtr&>(wrapper).Set(obj); }
		bool IsNull(const FZSelfDescriptiveSoftObjectPtr& wrapper) { return wrapper.GetUnderlyingInstance()->IsNull(); }
		UObject* Load(const FZSelfDescriptiveSoftObjectPtr& wrapper) { return wrapper.GetUnderlyingInstance()->LoadSynchronous(); }
	}
	
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_SoftObject_Private::Get, FSoftObject::Get)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_SoftObject_Private::Set, FSoftObject::Set)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_SoftObject_Private::IsNull, FSoftObject::IsNull)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_SoftObject_Private::Load, FSoftObject::Load)
}


