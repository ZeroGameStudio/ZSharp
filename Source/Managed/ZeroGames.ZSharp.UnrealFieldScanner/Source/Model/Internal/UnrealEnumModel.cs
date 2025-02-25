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
		_values = typeDef.Fields.Where(field => field.Constant is not null).Select(field => (field.Name, Convert.ToInt64(field.Constant))).ToArray();
	}

	public string AssemblyName { get; }
	public string FullName { get; }
	
	public IModelRegistry Registry { get; }
	public TypeModelReference? BaseType { get; }
	public IReadOnlyList<InterfaceTypeUri> Interfaces { get; } = [];
	
	public bool IsFlags { get; }
	public IEnumerable<(string, int64)> Values => _values;

	private readonly (string, int64)[] _values;

}


