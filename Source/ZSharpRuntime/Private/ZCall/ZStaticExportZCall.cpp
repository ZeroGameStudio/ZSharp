// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticExportZCall.h"

#include "ZCallResolver_Export.h"

ZSharp::FZStaticExportZCall::FZStaticExportZCall(const FString& name, const TFunction<int32(FZCallBuffer*)>& function)
{
	FZCallResolver_Export::RegisterFunction(name, function);
}

FString ZSharp::ZStaticExportZCall_Private::MakeZCallName(const FString& functionName)
{
	FString left;
	FString right;
	functionName.RightChop(1).Split("::", &left, &right);
	check(!left.Contains("::"));
	check(!right.Contains("::"));

	return FString::Printf(TEXT("ex://%s.%s"), *left, *right);
}


