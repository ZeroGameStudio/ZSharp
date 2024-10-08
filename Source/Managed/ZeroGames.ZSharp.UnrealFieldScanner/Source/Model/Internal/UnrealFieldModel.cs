// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal abstract class UnrealFieldModel : IUnrealFieldModel
{

	protected UnrealFieldModel(string name, ITypeResolver typeResolver, ICustomAttributeProvider attributeProvider)
	{
		Name = name;
		SpecifierResolver.Resolve(typeResolver, attributeProvider, _specifiers);
	}

	public virtual bool HasSpecifier(Type attributeType) => _specifiers.Any(spec => spec.GetType().IsAssignableTo(attributeType));
	public virtual IUnrealReflectionSpecifier? GetSpecifier(Type attributeType) => _specifiers.FirstOrDefault(spec => spec.GetType().IsAssignableTo(attributeType));
	
	public string Name { get; }

	public virtual IReadOnlyCollection<IUnrealReflectionSpecifier> Specifiers => _specifiers;

	private readonly List<IUnrealReflectionSpecifier> _specifiers = new();
	
}


