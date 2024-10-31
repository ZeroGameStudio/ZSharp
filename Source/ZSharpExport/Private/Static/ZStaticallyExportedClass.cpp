// Copyright Zero Games. All Rights Reserved.


#include "Static/ZStaticallyExportedClass.h"
#include "Static/ZStaticallyExportedClassMacros.h"

#include "ZExportedTypeRegistry.h"

bool ZSharp::ZStaticallyExportedClass_Private::RegisterClass(IZExportedClass* cls)
{
	return FZExportedTypeRegistry::Get().RegisterClass(cls);
}

ZSHARP_BEGIN_EXPORT_CLASS(FString)
ZSHARP_END_EXPORT_CLASS(FString)

ZSHARP_BEGIN_EXPORT_CLASS(FName)
ZSHARP_END_EXPORT_CLASS(FName)

ZSHARP_BEGIN_EXPORT_CLASS(FText)
ZSHARP_END_EXPORT_CLASS(FText)

ZSHARP_BEGIN_EXPORT_CLASS(FFieldPath)
ZSHARP_END_EXPORT_CLASS(FFieldPath)


