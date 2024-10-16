// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public abstract class MemberDefinitionBase(EMemberVisibility visibility, string name) : ITypeMember
{
	public EMemberVisibility Visibility { get; } = visibility;
	public EMemberModifiers Modifiers { get; set; }
	public string Name { get; } = name;
}


