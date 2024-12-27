// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

internal class MethodBodyGenerator
{
	
	public string Generate(MethodBody body, bool singleLine)
	{
		if (body.Contents.Length == 0)
		{
			return "{}";
		}

		if (body.Contents.Length == 1 && singleLine)
		{
			return $" => {body.Contents[0]}";
		}
		
		return
$@"
{{
{string.Join($"{Environment.NewLine}{Environment.NewLine}", body.Contents.Where(block => !string.IsNullOrWhiteSpace(block.Content))).Indent()}
}}";
	}
	
}


