// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Method)]
public class AsFieldNotifyAttribute : FunctionSpecifierBase
{
	public override IEnumerable<Type> HierarchicalRequirements => [ typeof(BlueprintPureAttribute) ];
}


