// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public readonly struct TypeModelReference : ITypeModel
{

	public TypeModelReference(ITypeModel model)
	{
		Registry = model.Registry;
		FullName = model.FullName;
	}

	public TypeModelReference(IModelRegistry registry, string fullName)
	{
		Registry = registry;
		FullName = fullName;
	}

	internal TypeModelReference(ModelRegistry registry, TypeReference typeRef) : this(registry.GetOrAddTypeModel(typeRef)){}
	
	public bool HasSpecifier(Type attributeType, bool exactType) => Type.HasSpecifier(attributeType, exactType);
	public IUnrealReflectionSpecifier? GetSpecifier(Type attributeType, bool exactType) => Type.GetSpecifier(attributeType, exactType);
	
	public IModelRegistry Registry { get; init; }
	public string AssemblyName => Type.AssemblyName;
	public string FullName { get; init; }
	public TypeModelReference? BaseType => Type.BaseType;
	public IReadOnlyList<InterfaceTypeUri> Interfaces => Type.Interfaces;

	public IReadOnlyCollection<IUnrealReflectionSpecifier> Specifiers => Type.Specifiers;
	
	public bool IsByRef { get; init; }
	public bool IsOut { get; init; }
	public IReadOnlyList<TypeModelReference>? GenericArguments { get; init; }

	public ITypeModel Type => Registry.GetTypeModel(FullName);

}


