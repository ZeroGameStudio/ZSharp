// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Method)]
public class SealedEventAttribute : Attribute, IUnrealReflectionSpecifier
{
	public IEnumerable<Type> HierarchicalRequirements => [ typeof(BlueprintRemoteFunctionSpecifierBase) ];
}


