// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class PlainTypeModel : ITypeModel
{

	public PlainTypeModel(ModelRegistry registry, TypeDefinition typeDef)
	{
		Registry = registry;
		AssemblyName = typeDef.Scope.GetAssemblyName();
		FullName = typeDef.FullName;
		if (typeDef.BaseType is not null)
		{
			BaseType = new(registry, typeDef.BaseType);
		}
		Interfaces = typeDef.Interfaces.Select(i => new InterfaceTypeUri(i.InterfaceType.Scope.GetAssemblyName(), i.InterfaceType.FullName)).ToArray();
		
		SpecifierResolver.Resolve(registry, typeDef, _specifiers);
	}
	
	public bool HasSpecifier(Type attributeType, bool exactType) => _specifiers.Any(spec => exactType ? spec.GetType() == attributeType : spec.GetType().IsAssignableTo(attributeType));
	public IUnrealReflectionSpecifier? GetSpecifier(Type attributeType, bool exactType) => _specifiers.FirstOrDefault(spec => exactType ? spec.GetType() == attributeType : spec.GetType().IsAssignableTo(attributeType));
	
	public IModelRegistry Registry { get; }
	public string AssemblyName { get; }
	public string FullName { get; }
	public TypeModelReference? BaseType { get; }
	public IReadOnlyList<InterfaceTypeUri> Interfaces { get; }

	public IReadOnlyCollection<IUnrealReflectionSpecifier> Specifiers => _specifiers;
	
	private readonly List<IUnrealReflectionSpecifier> _specifiers = new();
	
}


