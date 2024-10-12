// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class BlueprintNonDelegatePropertyVisibilitySpecifierBase : BlueprintPropertyVisibilitySpecifierBase;

public abstract class BlueprintPropertyReadWriteSpecifierBase : BlueprintNonDelegatePropertyVisibilitySpecifierBase;

public class BlueprintReadWriteAttribute : BlueprintPropertyReadWriteSpecifierBase;
public class BlueprintReadOnlyAttribute : BlueprintPropertyReadWriteSpecifierBase;

public abstract class BlueprintPropertyAccessorSpecifierBase(string accessorName) : BlueprintNonDelegatePropertyVisibilitySpecifierBase
{
	public override IEnumerable<Type> HierarchicalConflicts => [ typeof(BlueprintDelegatePropertyVisibilitySpecifierBase), typeof(BlueprintPropertyReadWriteSpecifierBase) ];

	public string AccessorName { get; } = accessorName;
}

public class BlueprintGetterAttribute(string accessorName) : BlueprintPropertyAccessorSpecifierBase(accessorName);
public class BlueprintSetterAttribute(string accessorName) : BlueprintPropertyAccessorSpecifierBase(accessorName);


