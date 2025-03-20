// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class SpecifierProcessor
{
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealStructDefinition def, IUnrealStructModel model, PropertyDefaultOverrideAttribute specifier)
	{
		if (specifier.Default.ToString() is {} defaultValue && !string.IsNullOrWhiteSpace(defaultValue))
		{
			def.PropertyDefaults.Add(new()
			{
				PropertyChain = specifier.Property,
				Buffer = defaultValue,
			});
		}
	}
	
}


