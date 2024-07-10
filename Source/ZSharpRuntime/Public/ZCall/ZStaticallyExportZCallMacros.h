// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZStaticallyExportZCall.h"

#define ZSHARP_STATIC_EXPORT_ZCALL_COMBINE_INNER(A, B) A##B
#define ZSHARP_STATIC_EXPORT_ZCALL_COMBINE(A, B) ZSHARP_STATIC_EXPORT_ZCALL_COMBINE_INNER(A, B)

#define ZSHARP_STATIC_EXPORT_ZCALL_EX(Function, Name) namespace __ZStaticallyExportZCall_Private { static TZStaticallyExportZCall<decltype(&Function), &Function> ZSHARP_STATIC_EXPORT_ZCALL_COMBINE(__GStaticallyExportedZCall, __LINE__) { ZSharp::ZStaticallyExportZCall_Private::MakeZCallName(TEXT(#Name)) }; }
#define ZSHARP_STATIC_EXPORT_ZCALL(Function) ZSHARP_STATIC_EXPORT_ZCALL_EX(Function, Function)


