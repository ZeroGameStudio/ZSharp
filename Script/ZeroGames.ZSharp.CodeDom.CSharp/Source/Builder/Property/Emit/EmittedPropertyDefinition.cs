// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct EmittedPropertyDefinition(PropertyDefinition property, FieldDefinition zcallHandle)
{
	public PropertyDefinition Property { get; } = property;
	public FieldDefinition ZCallHandle { get; } = zcallHandle;
}


