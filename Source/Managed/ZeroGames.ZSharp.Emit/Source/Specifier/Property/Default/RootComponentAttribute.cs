// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Property)]
public class RootComponentAttribute : Attribute, IUnrealReflectionSpecifier
{
	public IEnumerable<Type> HierarchicalRequirements => [ typeof(DefaultSubobjectAttribute) ];
}


