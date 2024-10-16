// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class ClassDefinition(bool isInterface, EMemberVisibility visibility, string name) : CompositeTypeDefinitionBase(visibility, name)
{
	public override ETypeKind Kind { get; } = isInterface ? ETypeKind.Interface : ETypeKind.Class;
}


