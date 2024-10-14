// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public class CustomThunkAttribute(string thunkName) : FunctionSpecifierBase
{
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(EventSpecifierBase) ];

	public string ThunkName { get; } = thunkName;
}


