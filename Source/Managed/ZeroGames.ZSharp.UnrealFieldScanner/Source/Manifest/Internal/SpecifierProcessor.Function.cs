// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class SpecifierProcessor
{
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, UFunctionAttribute specifier)
	{
		if (specifier.Name is { } overrideName)
		{
			def.ZCallName = $"m://{model.Outer.AssemblyName}:{model.Outer.FullName}:{overrideName}";
		}
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
		throw new NotImplementedException();
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, BlueprintEventOverrideAttribute specifier)
	{
		def.Name = specifier.Event;
		def.FunctionFlags |= EFunctionFlags.FUNC_BlueprintEvent;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, SealedEventAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.FUNC_Final;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, ServerAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.FUNC_Event | EFunctionFlags.FUNC_Net | EFunctionFlags.FUNC_NetServer;
		
		throw new NotImplementedException();
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, ClientAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.FUNC_Event | EFunctionFlags.FUNC_Net | EFunctionFlags.FUNC_NetClient;
		
		throw new NotImplementedException();
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, NetMulticastAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.FUNC_Event | EFunctionFlags.FUNC_Net | EFunctionFlags.FUNC_NetMulticast;
		
		throw new NotImplementedException();
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, WithValidationAttribute specifier)
	{
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
		throw new NotImplementedException();
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, ExecAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.FUNC_Exec;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, CustomThunkAttribute specifier)
	{
		throw new NotImplementedException();
	}
	
}


