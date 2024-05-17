// Copyright Zero Games. All Rights Reserved.


#include "Dynamic/ZDynamicExportedClass.h"

#include "ZExportedTypeRegistry.h"
#include "ZSharpExportHelpers.h"

ZSharp::FZDynamicExportedClass::FZDynamicExportedClass(UStruct* ustruct)
	: Struct(ustruct)
{
	check(Struct->IsA<UClass>() || Struct->IsA<UScriptStruct>());
	check(Struct->IsNative());

	bRegistered = FZExportedTypeRegistry::Get().RegisterClass(this);
}

FString ZSharp::FZDynamicExportedClass::GetName() const
{
	return FZSharpExportHelpers::GetUFieldAliasedName(Struct);
}

FString ZSharp::FZDynamicExportedClass::GetModule() const
{
	return FZSharpExportHelpers::GetUFieldModuleName(Struct);
}

FString ZSharp::FZDynamicExportedClass::GetOuterExportName() const
{
	return FZSharpExportHelpers::GetUFieldOuterExportName(Struct);
}

FString ZSharp::FZDynamicExportedClass::GetBaseType() const
{
	const UStruct* super = Struct->GetSuperStruct();
	if (!super)
	{
		if (Struct->IsA<UClass>())
		{
			return "@UCLASS";
		}
		else
		{
			return "@USTRUCT";
		}
	}

	return FZSharpExportHelpers::GetUFieldOuterExportName(super);
}

TArray<FString> ZSharp::FZDynamicExportedClass::GetInterfaces() const
{
	checkNoEntry();
	return {};
}

void ZSharp::FZDynamicExportedClass::ForeachMethod(TFunctionRef<void(IZExportedMethod&)> action) const
{
	checkNoEntry();
}


