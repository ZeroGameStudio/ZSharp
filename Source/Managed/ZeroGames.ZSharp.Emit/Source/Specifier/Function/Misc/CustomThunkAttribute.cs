// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class CustomThunkAttribute(string thunkName) : FunctionSpecifierBase
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintEventSpecifierBase) ];

	public string ThunkName { get; } = thunkName;
}


