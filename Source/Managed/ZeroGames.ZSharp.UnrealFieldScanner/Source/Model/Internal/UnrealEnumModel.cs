// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class UnrealEnumModel : UnrealFieldModel, IUnrealEnumModel
{

	public UnrealEnumModel(ModelRegistry registry, TypeDefinition typeDef) : base(typeDef.Name, registry, typeDef)
	{
		AssemblyName = typeDef.Scope.GetAssemblyName();
		FullName = typeDef.FullName;
		
		Registry = registry;
		BaseType = new(registry, typeDef.BaseType);

		IsFlags = typeDef.CustomAttributes.Any(attr => attr.AttributeType.FullName == typeof(FlagsAttribute).FullName);
		Fields = typeDef.Fields.Where(field => field.Constant is not null).Select(field => new UnrealEnumFieldModel(registry, field)).ToArray();
	}

	public string AssemblyName { get; }
	public string FullName { get; }
	
	public IModelRegistry Registry { get; }
	public TypeModelReference? BaseType { get; }
	public IReadOnlyList<InterfaceTypeUri> Interfaces { get; } = [];
	
	public bool IsFlags { get; }
	public IReadOnlyList<IUnrealEnumFieldModel> Fields { get; }

}


