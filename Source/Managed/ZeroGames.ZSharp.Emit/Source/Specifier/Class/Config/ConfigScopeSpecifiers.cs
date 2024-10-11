// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Class)]
public abstract class ConfigScopeSpecifierBase : Attribute, IUnrealReflectionSpecifier
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(ConfigScopeSpecifierBase) ];
}

public class DefaultConfigAttribute : ConfigScopeSpecifierBase;
public class ProjectUserConfigAttribute : ConfigScopeSpecifierBase;
public class GlobalUserConfigAttribute : ConfigScopeSpecifierBase;


