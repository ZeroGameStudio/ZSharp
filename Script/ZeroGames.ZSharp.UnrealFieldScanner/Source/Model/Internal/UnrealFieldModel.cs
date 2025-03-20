// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal abstract class UnrealFieldModel : IUnrealFieldModel
{
	
	protected UnrealFieldModel(string name)
	{
		Name = name;
	}

	protected UnrealFieldModel(string name, ModelRegistry modelRegistry, ICustomAttributeProvider attributeProvider) : this(name)
	{
		SpecifierResolver.Resolve(modelRegistry, attributeProvider, _specifiers);
	}

	public bool HasSpecifier(Type attributeType, bool exactType) => _specifiers.Any(spec => exactType ? spec.GetType() == attributeType : spec.GetType().IsAssignableTo(attributeType));
	public IUnrealReflectionSpecifier? GetSpecifier(Type attributeType, bool exactType) => _specifiers.FirstOrDefault(spec => exactType ? spec.GetType() == attributeType : spec.GetType().IsAssignableTo(attributeType));

	public string Name { get; }

	public virtual IReadOnlyCollection<IUnrealReflectionSpecifier> Specifiers => _specifiers;

	protected readonly List<IUnrealReflectionSpecifier> _specifiers = [];
	
}


