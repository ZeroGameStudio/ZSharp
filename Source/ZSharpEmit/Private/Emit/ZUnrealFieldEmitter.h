// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Field/ZUnrealFieldDefinitions.h"

namespace ZSharp
{
	class FZUnrealFieldEmitter
	{
		
	public:
		static FZUnrealFieldEmitter& Get();

	public:
		UPackage* Emit(FZPackageDefinition& def);

	private:
		void EmitPackage(FZPackageDefinition& def) const;
		
		void EmitEnum(UPackage* pak, FZEnumDefinition& def) const;
		
		void EmitStructSkeleton(UPackage* pak, FZScriptStructDefinition& def) const;
		void EmitClassSkeleton(UPackage* pak, FZClassDefinition& def) const;
		void EmitInterfaceSkeleton(UPackage* pak, FZInterfaceDefinition& def) const;
		void EmitDelegateSkeleton(UPackage* pak, FZDelegateDefinition& def) const;
		
		void FinishEmitStruct(UPackage* pak, const FZScriptStructDefinition& def) const;
		void FinishEmitClass(UPackage* pak, const FZClassDefinition& def) const;
		void FinishEmitInterface(UPackage* pak, const FZInterfaceDefinition& def) const;
		void FinishEmitDelegate(UPackage* pak, const FZDelegateDefinition& def) const;

		void AddMetadata(UObject* object, const TMap<FName, FString>& metadata) const;
		
	private:
		FZUnrealFieldEmitter() = default;

	private:
		bool bEmitting = false;
		
	};
}


