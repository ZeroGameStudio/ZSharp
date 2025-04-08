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
		def.FunctionFlags |= EFunctionFlags.BlueprintCallable;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, BlueprintPureAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.BlueprintCallable | EFunctionFlags.BlueprintPure;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, BlueprintAuthorityOnlyAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.BlueprintAuthorityOnly;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, BlueprintCosmeticAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.BlueprintCosmetic;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, BlueprintImplementableEventAttribute specifier)
	{
		check(model.Visibility != EMemberVisibility.Private);

		def.FunctionFlags &= ~EFunctionFlags.Native;
		def.FunctionFlags |= EFunctionFlags.Event | EFunctionFlags.BlueprintEvent;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, BlueprintNativeEventAttribute specifier)
	{
		check(model.Visibility != EMemberVisibility.Private);

		def.ZCallName = $"{def.Name}_Implementation";
		def.FunctionFlags |= EFunctionFlags.Event | EFunctionFlags.BlueprintEvent;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, SealedEventAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.Final;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, ServerAttribute specifier)
	{
		check(!model.HasReturnValue());
		
		def.ZCallName = $"{def.Name}_Implementation";
		def.FunctionFlags |= EFunctionFlags.Event | EFunctionFlags.Net | EFunctionFlags.NetServer;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, ClientAttribute specifier)
	{
		check(!model.HasReturnValue());
		
		def.ZCallName = $"{def.Name}_Implementation";
		def.FunctionFlags |= EFunctionFlags.Event | EFunctionFlags.Net | EFunctionFlags.NetClient;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, NetMulticastAttribute specifier)
	{
		check(!model.HasReturnValue());
		
		def.ZCallName = $"{def.Name}_Implementation";
		def.FunctionFlags |= EFunctionFlags.Event | EFunctionFlags.Net | EFunctionFlags.NetMulticast;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, WithValidationAttribute specifier)
	{
		def.ValidateZCallName = $"{def.Name}_Validate";
		def.FunctionFlags |= EFunctionFlags.NetValidate;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, ReliableAttribute specifier)
	{
		def.FunctionFlags |= EFunctionFlags.NetReliable;
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
		def.FunctionFlags |= EFunctionFlags.Exec;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, CustomThunkAttribute specifier)
	{
		def.CustomThunkName = specifier.ThunkName;
	}
	
	// Parameter
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, WorldContextAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.WorldContext, specifier.Name);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, DefaultToSelfAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.DefaultToSelf, specifier.Name);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, UClassConstructorAttribute specifier)
	{
		// Nothing to do.
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealFunctionDefinition def, IUnrealFunctionModel model, NetSerializeAttribute specifier)
	{
		// Nothing to do.
	}
	
}


