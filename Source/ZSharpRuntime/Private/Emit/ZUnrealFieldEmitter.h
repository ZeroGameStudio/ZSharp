// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZEnumDefinition;
	struct FZScriptStructDefinition;
	struct FZClassDefinition;
	struct FZInterfaceDefinition;
	struct FZDelegateDefinition;
	struct FZUnrealFieldManifest;
	
	class FZUnrealFieldEmitter
	{
		
	public:
		static FZUnrealFieldEmitter& Get();

	public:
		void Emit(FZUnrealFieldManifest& manifest);

	private:
		void InternalEmit(FZUnrealFieldManifest& manifest) const;
		
		void EmitEnum(UPackage* pak, FZEnumDefinition& def) const;
		
		void EmitStructSkeleton(UPackage* pak, FZScriptStructDefinition& def) const;
		void EmitClassSkeleton(UPackage* pak, FZClassDefinition& def) const;
		void EmitInterfaceSkeleton(UPackage* pak, FZInterfaceDefinition& def) const;
		void EmitDelegateSkeleton(UPackage* pak, FZDelegateDefinition& def) const;
		
		void FinishEmitStruct(UPackage* pak, FZScriptStructDefinition& def) const;
		void FinishEmitClass(UPackage* pak, FZClassDefinition& def) const;
		void FinishEmitInterface(UPackage* pak, FZInterfaceDefinition& def) const;
		void FinishEmitDelegate(UPackage* pak, FZDelegateDefinition& def) const;

	private:
		FZUnrealFieldEmitter() = default;

	private:
		bool bEmitting = false;
		TSet<FName> EmittedModules;
		
	};
}


