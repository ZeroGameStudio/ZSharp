// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public abstract class CompositeTypeDefinitionBase(EMemberVisibility visibility, string name) : TypeDefinitionBase(visibility, name)
{
	
	public void AddMember(ITypeMember member) => _members.Add(member);

	public IReadOnlyList<ITypeMember> Members => _members;

	private readonly List<ITypeMember> _members = new();

}


