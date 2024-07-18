// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp
{
	namespace ZCallExport_SoftClass_Private
	{
		UObject* Get(const FZSelfDescriptiveSoftClassPtr& wrapper) { return wrapper.Get(); }
		// Here it must be const ref, otherwise it will try to copy out after function call.
		void Set(const FZSelfDescriptiveSoftClassPtr& wrapper, UClass* cls) { const_cast<FZSelfDescriptiveSoftClassPtr&>(wrapper).Set(cls); }
		bool IsNull(const FZSelfDescriptiveSoftClassPtr& wrapper) { return wrapper.GetUnderlyingInstance()->IsNull(); }
		UObject* Load(const FZSelfDescriptiveSoftClassPtr& wrapper) { return wrapper.GetUnderlyingInstance()->LoadSynchronous(); }
	}
	
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_SoftClass_Private::Get, FSoftClass::Get)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_SoftClass_Private::Set, FSoftClass::Set)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_SoftClass_Private::IsNull, FSoftClass::IsNull)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(ZCallExport_SoftClass_Private::Load, FSoftClass::Load)
}


