// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class PropertyConfigSpecifierBase: PropertySpecifierBase
{
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(PropertyConfigSpecifierBase) ];
}

public class ConfigAttribute : PropertyConfigSpecifierBase;
public class GlobalConfigAttribute : PropertyConfigSpecifierBase;


