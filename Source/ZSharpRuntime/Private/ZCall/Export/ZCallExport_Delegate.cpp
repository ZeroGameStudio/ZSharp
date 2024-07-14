// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticallyExportZCall.h"
#include "ZCall/ZStaticallyExportZCallMacros.h"

namespace ZSharp
{
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptDelegate::BindUFunction, FDelegate::BindUFunction)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptDelegate::BindManaged, FDelegate::BindManaged)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptDelegate::Unbind, FDelegate::Unbind)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptDelegate::GetObject, FDelegate::GetObject)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptDelegate::GetZCallName, FDelegate::GetZCallName)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptDelegate::IsBound, FDelegate::IsBound)
	ZSHARP_STATIC_EXPORT_ZCALL_EX(FZSelfDescriptiveScriptDelegate::IsBoundToObject, FDelegate::IsBoundToObject)
}


