// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class MethodDefinition(EMemberVisibility visibility, string name, TypeReference? returnType, params ParameterDeclaration[]? parameters) : MemberDefinitionBase(visibility, name)
{

	public MethodSignature Signature { get; } = new(returnType, parameters);
	public MethodBody? Body { get; set; }
	
	public bool IsDelegate { get; set; }
	
}


