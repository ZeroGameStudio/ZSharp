// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class PropertyDefinition(EMemberVisibility visibility, string name, string type) : MemberDefinitionBase(visibility, name)
{
	public string Type { get; } = type;
	public MethodBody? Getter { get; set; }
	public MethodBody? Setter { get; set; }
}


