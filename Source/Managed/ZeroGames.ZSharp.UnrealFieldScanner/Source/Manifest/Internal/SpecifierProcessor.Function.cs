// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class SpecifierProcessor
{
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, UFunctionAttribute specifier)
	{
		if (specifier.Name is { } overrideName)
		{
			def.Name = overrideName;
			def.ZCallName = $"m://{model.Outer.AssemblyName}:{model.Outer.FullName}:{overrideName}";
		}
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, BlueprintEventAttribute specifier)
	{
		Logger.Warning($"Specifier {specifier.GetType().FullName} is not implemented yet.");
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, BlueprintEventOverrideAttribute specifier)
	{
		def.Name = specifier.EventName;
		def.FunctionFlags |= EFunctionFlags.FUNC_BlueprintEvent;
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
	
}


