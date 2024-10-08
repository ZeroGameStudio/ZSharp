// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class PropertyAccessorModel : IPropertyAccessorModel
{
	
	public PropertyAccessorModel(MethodDefinition methodDef, ITypeResolver typeResolver)
	{
		Visibility = methodDef.IsPublic ? EMemberVisibility.Public : methodDef.IsFamily ? EMemberVisibility.Protected : EMemberVisibility.Private;
		SpecifierResolver.Resolve(typeResolver, methodDef, _specifiers);
	}
	
	public bool HasSpecifier(Type attributeType) => _specifiers.Any(spec => spec.GetType().IsAssignableTo(attributeType));
	public IUnrealReflectionSpecifier? GetSpecifier(Type attributeType) => _specifiers.FirstOrDefault(spec => spec.GetType().IsAssignableTo(attributeType));

	public EMemberVisibility Visibility { get; }
	public IReadOnlyCollection<IUnrealReflectionSpecifier> Specifiers => _specifiers;
	
	private readonly List<IUnrealReflectionSpecifier> _specifiers = new();
	
}


