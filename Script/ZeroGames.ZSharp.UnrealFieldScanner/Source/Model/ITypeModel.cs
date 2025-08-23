// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface ITypeModel : ITypeUri, ISpecifierProvider, IScanTimeType
{
	IModelRegistry Registry { get; }
	TypeModelReference? BaseType { get; }
	TypeModelReference EnumUnderlyingType { get; }
	IReadOnlyList<InterfaceTypeUri> Interfaces { get; }
}

public static class TypeModelExtensions
{
	extension(ITypeModel @this)
	{
		public bool IsObjectRootType => @this.FullName == typeof(object).FullName;
		public bool IsEnum => @this.BaseType?.FullName == typeof(Enum).FullName;
		public string? UnrealFieldPath => @this.GetSpecifier<UnrealFieldPathAttribute>()?.Path;
	}
}


