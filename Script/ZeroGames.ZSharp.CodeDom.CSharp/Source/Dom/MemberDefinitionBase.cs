// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public abstract class MemberDefinitionBase(EMemberVisibility visibility, string name) : ITypeMember
{
	
	public void AddAttributeListBefore(params AttributeDeclaration[] declarations) => _attributes.Insert(0, new(declarations));
	public void AddAttributeListAfter(params AttributeDeclaration[] declarations) => _attributes.Add(new(declarations));
	
	public EMemberVisibility Visibility { get; } = visibility;
	public EMemberModifiers Modifiers { get; set; }
	public string Name { get; } = name;
	public IReadOnlyList<AttributeList> Attributes => _attributes;
	
	private readonly List<AttributeList> _attributes = new();
	
}


