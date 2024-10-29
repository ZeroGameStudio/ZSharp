// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class SpecifierProcessor
{
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, UFunctionAttribute specifier)
	{
		// Nothing to do.
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, BlueprintCallableAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.FUNC_BlueprintCallable;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, BlueprintPureAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.FUNC_BlueprintCallable;
		def.FunctionFlags |= EFunctionFlags.FUNC_BlueprintPure;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, BlueprintAuthorityOnlyAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.FUNC_BlueprintCallable;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, BlueprintCosmeticAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.FUNC_BlueprintCallable;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, BlueprintEventAttribute specifier)
	{
		check(model.Visibility != EMemberVisibility.Private);

		string name = specifier.Implementation ?? $"{def.Name}_Implementation";
		def.ZCallName = $"m://{model.Outer.AssemblyName}:{model.Outer.FullName}:{name}";
		def.FunctionFlags |= EFunctionFlags.FUNC_Event | EFunctionFlags.FUNC_BlueprintEvent;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, SealedEventAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.FUNC_Final;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, ServerAttribute specifier)
	{
		check(!model.HasReturnValue());
		
		string name = specifier.Implementation ?? $"{def.Name}_Implementation";
		def.ZCallName = $"m://{model.Outer.AssemblyName}:{model.Outer.FullName}:{name}";
		def.FunctionFlags |= EFunctionFlags.FUNC_Event | EFunctionFlags.FUNC_Net | EFunctionFlags.FUNC_NetServer;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, ClientAttribute specifier)
	{
		check(!model.HasReturnValue());
		
		string name = specifier.Implementation ?? $"{def.Name}_Implementation";
		def.ZCallName = $"m://{model.Outer.AssemblyName}:{model.Outer.FullName}:{name}";
		def.FunctionFlags |= EFunctionFlags.FUNC_Event | EFunctionFlags.FUNC_Net | EFunctionFlags.FUNC_NetClient;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, NetMulticastAttribute specifier)
	{
		check(!model.HasReturnValue());
		
		string name = specifier.Implementation ?? $"{def.Name}_Implementation";
		def.ZCallName = $"m://{model.Outer.AssemblyName}:{model.Outer.FullName}:{name}";
		def.FunctionFlags |= EFunctionFlags.FUNC_Event | EFunctionFlags.FUNC_Net | EFunctionFlags.FUNC_NetMulticast;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, WithValidationAttribute specifier)
	{
		string name = specifier.Implementation ?? $"{def.Name}_Validate";
		def.ValidateZCallName = $"m://{model.Outer.AssemblyName}:{model.Outer.FullName}:{name}";
		def.FunctionFlags |= EFunctionFlags.FUNC_NetValidate;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, ReliableAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.FUNC_NetReliable;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, UnreliableAttribute specifier)
	{
		// Do nothing. This exists only for requirement checking.
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, AsFieldNotifyAttribute specifier)
	{
		def.Outer.FieldNotifies.Add(def.Name);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, ExecAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.FUNC_Exec;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, CustomThunkAttribute specifier)
	{
		def.CustomThunkName = specifier.ThunkName;
	}
	
}


