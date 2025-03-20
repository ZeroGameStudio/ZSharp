// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class FieldDefinition(EMemberVisibility visibility, string name, TypeReference type) : MemberDefinitionBase(visibility, name)
{
	public TypeReference Type { get; } = type;
}


