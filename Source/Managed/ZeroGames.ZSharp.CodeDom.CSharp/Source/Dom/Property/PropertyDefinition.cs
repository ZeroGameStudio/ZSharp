// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class PropertyDefinition(EMemberVisibility visibility, string name, TypeReference type, bool hasGetter, bool hasSetter) : MemberDefinitionBase(visibility, name)
{
	public TypeReference Type { get; } = type;
	public bool HasGetter { get; } = hasGetter;
	public bool HasSetter { get; } = hasSetter;
	public MethodBody? Getter { get; set; }
	public MethodBody? Setter { get; set; }
}


