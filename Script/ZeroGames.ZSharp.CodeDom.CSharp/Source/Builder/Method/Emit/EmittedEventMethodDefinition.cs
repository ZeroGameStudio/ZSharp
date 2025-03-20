// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public readonly struct EmittedEventMethodDefinition(MethodDefinition stub, MethodDefinition? implementation, MethodDefinition? validation, FieldDefinition zcallHandle)
{
	public MethodDefinition Stub { get; } = stub;
	public MethodDefinition? Implementation { get; } = implementation;
	public MethodDefinition? Validation { get; } = validation;
	public FieldDefinition ZCallHandle { get; } = zcallHandle;
}


