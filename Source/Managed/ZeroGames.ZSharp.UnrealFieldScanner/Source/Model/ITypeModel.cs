// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface ITypeModel : ITypeUri, ISpecifierProvider, IScanTimeType
{
	IModelRegistry Registry { get; }
	TypeModelReference? BaseType { get; }
	IReadOnlyList<InterfaceTypeUri> Interfaces { get; }
}

public static class TypeModelExtensions
{

	public static bool IsObjectRootType(this ITypeModel @this) => @this.FullName == typeof(object).FullName;
	public static bool IsEnum(this ITypeModel @this) => @this.BaseType?.FullName == typeof(Enum).FullName;
	
	public static string? GetUnrealFieldPath(this ITypeModel @this) => @this.GetSpecifier<UnrealFieldPathAttribute>()?.Path;

}


