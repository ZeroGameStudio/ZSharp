// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class PlainTypeModel : ITypeModel
{

	public PlainTypeModel(ModelRegistry registry, TypeDefinition typeDef)
	{
		Registry = registry;
		FullName = typeDef.FullName;
		BaseType = new(registry, typeDef.BaseType);
		
		SpecifierResolver.Resolve(registry, typeDef, _specifiers);
	}

	public bool HasSpecifier(Type attributeType) => _specifiers.Any(spec => spec.GetType().IsAssignableTo(attributeType));
	public IUnrealReflectionSpecifier? GetSpecifier(Type attributeType) => _specifiers.FirstOrDefault(spec => spec.GetType().IsAssignableTo(attributeType));

	public IModelRegistry Registry { get; }
	public string FullName { get; }
	public TypeModelReference BaseType { get; }

	public IReadOnlyCollection<IUnrealReflectionSpecifier> Specifiers => _specifiers;
	
	private readonly List<IUnrealReflectionSpecifier> _specifiers = new();
	
}


