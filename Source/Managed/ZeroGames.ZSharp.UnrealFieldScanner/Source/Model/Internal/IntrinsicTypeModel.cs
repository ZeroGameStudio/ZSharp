// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class IntrinsicTypeModel : ITypeModel
{

	public IntrinsicTypeModel(ModelRegistry registry, Type runtimeType)
	{
		Registry = registry;
		FullName = runtimeType.FullName!;
		if (runtimeType.BaseType is not null)
		{
			BaseType = new(registry.GetTypeModelUnchecked(runtimeType.BaseType.FullName!));
		}
	}

	public bool HasSpecifier(Type attributeType) => false;
	public IUnrealReflectionSpecifier? GetSpecifier(Type attributeType) => null;

	public IModelRegistry Registry { get; }
	public string FullName { get; }
	public TypeModelReference BaseType { get; }

	public IReadOnlyCollection<IUnrealReflectionSpecifier> Specifiers => Array.Empty<IUnrealReflectionSpecifier>();
	
}


