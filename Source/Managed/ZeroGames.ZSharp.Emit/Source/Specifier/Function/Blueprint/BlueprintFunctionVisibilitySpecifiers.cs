// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class BlueprintFunctionVisibilitySpecifierBase : FunctionSpecifierBase
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintFunctionVisibilitySpecifierBase) ];
}

public class BlueprintCallableAttribute : BlueprintFunctionVisibilitySpecifierBase;
public class BlueprintPureAttribute : BlueprintFunctionVisibilitySpecifierBase;


