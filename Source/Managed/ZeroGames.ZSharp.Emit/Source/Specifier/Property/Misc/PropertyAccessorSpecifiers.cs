// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class PropertyAccessorSpecifierBase : PropertySpecifierBase
{
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintPropertyAccessorSpecifierBase) ];
	
	public string? CustomThunk { get; init; }
}

public class GetterAttribute : PropertyAccessorSpecifierBase;
public class SetterAttribute : PropertyAccessorSpecifierBase;


