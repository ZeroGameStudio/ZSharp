// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class IntrinsicTypeModel : ITypeModel
{

	public IntrinsicTypeModel(ModelRegistry registry, Type runtimeType)
	{
		Registry = registry;
		AssemblyName = runtimeType.Assembly.GetName().Name!;
		FullName = runtimeType.FullName!;
		if (runtimeType.BaseType is not null)
		{
			BaseType = new(registry.GetTypeModelUnchecked(runtimeType.BaseType.FullName!));
		}
	}

	public bool HasSpecifier(Type attributeType, bool exactType) => false;
	public IUnrealReflectionSpecifier? GetSpecifier(Type attributeType, bool exactType) => null;

	public IModelRegistry Registry { get; }
	public string AssemblyName { get; }
	public string FullName { get; }
	public TypeModelReference? BaseType { get; }
	public TypeModelReference EnumUnderlyingType => throw new NotSupportedException();
	public IReadOnlyList<InterfaceTypeUri> Interfaces { get; } = [];

	public IReadOnlyCollection<IUnrealReflectionSpecifier> Specifiers { get; } = [];
	
}


