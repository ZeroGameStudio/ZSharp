// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class ConfigScopeSpecifierBase : ClassSpecifierBase
{
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(ConfigScopeSpecifierBase) ];
}

public class DefaultConfigAttribute : ConfigScopeSpecifierBase;
public class ProjectUserConfigAttribute : ConfigScopeSpecifierBase;
public class GlobalUserConfigAttribute : ConfigScopeSpecifierBase;


