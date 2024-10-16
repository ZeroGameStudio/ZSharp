// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

internal class MethodBodyGenerator
{
	
	public string Generate(MethodBody body)
	{
		return 
$@"{{
{body.Content.Indent()}
}}";
	}
	
}


