// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZStaticExportZCall.h"

#define ZSHARP_STATIC_EXPORT_ZCALL_COMBINE_INNER(A, B) A##B
#define ZSHARP_STATIC_EXPORT_ZCALL_COMBINE(A, B) ZSHARP_STATIC_EXPORT_ZCALL_COMBINE_INNER(A, B)

#define ZSHARP_STATIC_EXPORT_ZCALL_EX(Function, Name) namespace __ZStaticExportZCall_Private { static TZStaticExportZCall<decltype(&Function), &Function> ZSHARP_STATIC_EXPORT_ZCALL_COMBINE(__GStaticExportedZCall, __LINE__) { ZSharp::ZStaticExportZCall_Private::MakeZCallName(TEXT(#Function)) }; }
#define ZSHARP_STATIC_EXPORT_ZCALL(Function) ZSHARP_STATIC_EXPORT_ZCALL_EX(Function, Function)


