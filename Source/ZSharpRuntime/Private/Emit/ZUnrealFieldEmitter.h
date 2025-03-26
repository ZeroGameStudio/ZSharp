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

	/**
	 * !!!!!!!!!!!!!!!!!!!!! LOW LEVEL HACK !!!!!!!!!!!!!!!!!!!!!
	 * 
	 * VERY, VERY, VERY IMPORTANT:
	 * THIS RELIES HEAVILY ON IMPLEMENTATION DETAILS OF UNREAL ENGINE,
	 * ESPECIALLY THE CONSTRUCTION PROCESS OF METADATA (REFLECTION).
	 * THEREFORE, UPGRADING THE ENGINE VERSION OR MODIFYING THE CODE IS HIGHLY LIKELY TO BREAK SOMETHING UP.
	 * NEVER TOUCH THIS CODE UNLESS YOU EXTREMELY, EXTREMELY, EXTREMELY KNOW WHAT YOU ARE DOING!!!
	 *
	 * !!!!!!!!!!!!!!!!!!!!! LOW LEVEL HACK !!!!!!!!!!!!!!!!!!!!!
	 */
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
		void EmitDelegateSkeleton(UPackage* pak, FZDelegateDefinition& def) const;
		void EmitInterfaceSkeleton(UPackage* pak, FZInterfaceDefinition& def) const;
		void EmitClassSkeleton(UPackage* pak, FZClassDefinition& def) const;
		
		void FinishEmitStruct(UPackage* pak, FZScriptStructDefinition& def) const;
		void FinishEmitDelegate(UPackage* pak, FZDelegateDefinition& def) const;
		void FinishEmitInterface(UPackage* pak, FZInterfaceDefinition& def) const;
		void FinishEmitClass(UPackage* pak, FZClassDefinition& def) const;

		void PostEmitStruct(UPackage* pak, FZScriptStructDefinition& def) const;
		void PostEmitDelegate(UPackage* pak, FZDelegateDefinition& def) const;
		void PostEmitClass_I(UPackage* pak, FZClassDefinition& def) const;
		void PostEmitClass_II(UPackage* pak, FZClassDefinition& def) const;

	private:
		FZUnrealFieldEmitter() = default;

	private:
		bool bEmitting = false;
		TSet<FName> EmittedModules;
		
	};
}


