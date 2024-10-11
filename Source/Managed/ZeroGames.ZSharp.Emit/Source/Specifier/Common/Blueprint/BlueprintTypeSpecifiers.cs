// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(SpecifierTargetConstants.AllFields)]
public abstract class BlueprintTypeSpecifierBase : Attribute, IUnrealReflectionSpecifier
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintTypeSpecifierBase) ];
}

public class BlueprintTypeAttribute : BlueprintTypeSpecifierBase;
public class NotBlueprintTypeAttribute : BlueprintTypeSpecifierBase;
public class BlueprintableAttribute : BlueprintTypeSpecifierBase;
public class NotBlueprintableAttribute : BlueprintTypeSpecifierBase;
public class BlueprintInternalUseOnlyAttribute : BlueprintTypeSpecifierBase;
public class BlueprintInternalUseOnlyHierarchicalAttribute : BlueprintTypeSpecifierBase;


