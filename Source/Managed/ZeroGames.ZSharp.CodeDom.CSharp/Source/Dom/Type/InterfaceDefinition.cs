// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class InterfaceDefinition(EMemberVisibility visibility, string name) : CompositeTypeDefinitionBase(visibility, name)
{
	public override ETypeKind Kind => ETypeKind.Interface;
}


