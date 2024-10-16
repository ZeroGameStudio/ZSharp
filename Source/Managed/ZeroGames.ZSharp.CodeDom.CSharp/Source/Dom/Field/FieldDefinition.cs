// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class FieldDefinition(EMemberVisibility visibility, string name, string type) : MemberDefinitionBase(visibility, name)
{
	public string Type { get; } = type;
}


