// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class BlueprintFunctionAuthenticationSpecifierBase : FunctionSpecifierBase
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintFunctionAuthenticationSpecifierBase) ];
}

public class BlueprintAuthorityOnlyAttribute : BlueprintFunctionAuthenticationSpecifierBase;
public class BlueprintCosmeticAttribute : BlueprintFunctionAuthenticationSpecifierBase;


