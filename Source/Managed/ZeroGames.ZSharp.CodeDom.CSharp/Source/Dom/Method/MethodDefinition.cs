// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class MethodDefinition(EMemberVisibility visibility, string name) : MemberDefinitionBase(visibility, name)
{

	public string? ReturnType { get; set; }
	public ParameterList? Parameters { get; set; }
	public MethodBody? Body { get; set; }
	
	public bool IsDelegate { get; set; }
	
}


