// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

internal class MethodBodyGenerator
{
	
	public string Generate(MethodBody body)
	{
		return
$@"{{
{string.Join($"{Environment.NewLine}{Environment.NewLine}", body.Contents.Where(block => !string.IsNullOrWhiteSpace(block.Content))).Indent()}
}}";
	}
	
}


