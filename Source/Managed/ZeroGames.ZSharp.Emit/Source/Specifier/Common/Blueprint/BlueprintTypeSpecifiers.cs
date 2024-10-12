// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class BlueprintTypeSpecifierBase : CommonSpecifierBase
{
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintTypeSpecifierBase) ];
}

public class BlueprintTypeAttribute : BlueprintTypeSpecifierBase;
public class NotBlueprintTypeAttribute : BlueprintTypeSpecifierBase;
public class BlueprintableAttribute : BlueprintTypeSpecifierBase;
public class NotBlueprintableAttribute : BlueprintTypeSpecifierBase;
public class BlueprintInternalUseOnlyAttribute : BlueprintTypeSpecifierBase;
public class BlueprintInternalUseOnlyHierarchicalAttribute : BlueprintTypeSpecifierBase;


