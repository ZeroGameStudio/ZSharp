// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class BlueprintDelegatePropertyVisibilitySpecifierBase : BlueprintPropertyVisibilitySpecifierBase
{
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintNonDelegatePropertyVisibilitySpecifierBase) ];
}

public class BlueprintAssignableDelegateAttribute : BlueprintDelegatePropertyVisibilitySpecifierBase;
public class BlueprintCallableDelegateAttribute : BlueprintDelegatePropertyVisibilitySpecifierBase;
public class BlueprintAuthorityOnlyDelegateAttribute : BlueprintDelegatePropertyVisibilitySpecifierBase;


