// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class EditSpecifierBase : PropertySpecifierBase
{
	public IEnumerable<Type> HierarchicalConflicts => [ typeof(EditSpecifierBase) ];
}

public class EditAnywhereAttribute : EditSpecifierBase;
public class EditDefaultsOnlyAttribute : EditSpecifierBase;
public class EditInstanceOnlyAttribute : EditSpecifierBase;
public class VisibleAnywhereAttribute : EditSpecifierBase;
public class VisibleDefaultsOnlyAttribute : EditSpecifierBase;
public class VisibleInstanceOnlyAttribute : EditSpecifierBase;


