// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public abstract class TypeDefinitionBase(EMemberVisibility visibility, string name) : MemberDefinitionBase(visibility, name)
{
	
	public void AddBaseType(string baseType) => _baseTypes.Add(baseType);
	public void AddAttributeList(params AttributeDeclaration[] declarations) => _attributes.Add(new(declarations));
	
	public abstract ETypeKind Kind { get; }
	public IReadOnlyList<string> BaseTypes => _baseTypes;
	public IReadOnlyList<AttributeList> Attributes => _attributes;
	
	private readonly List<string> _baseTypes = new();
	private readonly List<AttributeList> _attributes = new();

}


