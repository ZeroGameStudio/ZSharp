// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class UsingStatement(string target)
{
	
	public string Target { get; } = target;
	
	public bool IsGlobal { get; set; }
	public bool IsStatic { get; set; }
	
	public string? Alias { get; set; }
	
}


