// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

[Flags]
public enum ETypeModifiers
{
	None = 0,
	
	Abstract = 1 << 0,
	Sealed = 1 << 1,
	Static = 1 << 2,
	Unsafe = 1 << 3,
	Partial = 1 << 4,
}

public abstract class TypeDefinitionBase(EMemberVisibility visibility, string name)
{
	
	public void AddBaseType(string baseType) => _baseTypes.Add(baseType);
	public void AddAttributeList(params AttributeDeclaration[] declarations) => _attributes.Add(new(declarations));

	public EMemberVisibility Visibility { get; set; } = visibility;
	public ETypeModifiers Modifiers { get; set; }
	public abstract ETypeKind Kind { get; }
	public string Name { get; set; } = name;
	public IReadOnlyList<string> BaseTypes => _baseTypes;
	public IReadOnlyList<AttributeList> Attributes => _attributes;
	
	private readonly List<string> _baseTypes = new();
	private readonly List<AttributeList> _attributes = new();

}


