// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public abstract class TypeDefinitionBase(EMemberVisibility visibility, string name) : MemberDefinitionBase(visibility, name)
{
	
	public void AddBaseType(string baseType) => _baseTypes.Add(baseType);
	
	public abstract ETypeKind Kind { get; }
	public IReadOnlyList<string> BaseTypes => _baseTypes;
	
	private readonly List<string> _baseTypes = new();

}


