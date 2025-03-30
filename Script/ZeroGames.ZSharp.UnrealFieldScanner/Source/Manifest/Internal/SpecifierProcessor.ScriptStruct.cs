// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class SpecifierProcessor
{
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealScriptStructDefinition def, IUnrealScriptStructModel model, UStructAttribute specifier)
	{
		// Nothing to do.
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealScriptStructDefinition def, IUnrealScriptStructModel model, AtomicAttribute specifier)
	{
		def.StructFlags |= EStructFlags.Atomic;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealScriptStructDefinition def, IUnrealScriptStructModel model, ImmutableAttribute specifier)
	{
		def.StructFlags |= EStructFlags.Atomic | EStructFlags.Immutable;
	}
	
}


