// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class PropertyAccessorModel : IPropertyAccessorModel
{
	
	public PropertyAccessorModel(MethodDefinition methodDef, ModelRegistry modelRegistry)
	{
		Visibility = methodDef.IsPublic ? EMemberVisibility.Public : methodDef.IsFamily ? EMemberVisibility.Protected : EMemberVisibility.Private;
		SpecifierResolver.Resolve(modelRegistry, methodDef, _specifiers);
	}
	
	public bool HasSpecifier(Type attributeType, bool exactType) => _specifiers.Any(spec => exactType ? spec.GetType() == attributeType : spec.GetType().IsAssignableTo(attributeType));
	public IUnrealReflectionSpecifier? GetSpecifier(Type attributeType, bool exactType) => _specifiers.FirstOrDefault(spec => exactType ? spec.GetType() == attributeType : spec.GetType().IsAssignableTo(attributeType));
	
	public EMemberVisibility Visibility { get; }
	public IReadOnlyCollection<IUnrealReflectionSpecifier> Specifiers => _specifiers;
	
	private readonly List<IUnrealReflectionSpecifier> _specifiers = new();
	
}


