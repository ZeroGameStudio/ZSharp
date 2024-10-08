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
	
	public bool HasSpecifier(Type attributeType) => Type.HasSpecifier(attributeType);
	public IUnrealReflectionSpecifier? GetSpecifier(Type attributeType) => Type.GetSpecifier(attributeType);
	
	public IModelRegistry Registry { get; init; }
	public string FullName { get; init; }
	public TypeModelReference BaseType => Type.BaseType;
	
	public IReadOnlyCollection<IUnrealReflectionSpecifier> Specifiers => Type.Specifiers;
	
	public bool IsByRef { get; init; }
	public bool IsOut { get; init; }
	public IReadOnlyList<TypeModelReference>? GenericArguments { get; init; }

	public ITypeModel Type => Registry.GetTypeModel(FullName);

}


