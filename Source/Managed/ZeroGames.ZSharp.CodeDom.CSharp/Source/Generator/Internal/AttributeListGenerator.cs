// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

internal class AttributeListGenerator
{

	public string Generate(AttributeList attributeList)
	{
		var attributeDeclarations = attributeList.Declarations.Select(GenerateDeclaration);
		return $"[{string.Join(", ", attributeDeclarations)}]";
	}

	private string GenerateDeclaration(AttributeDeclaration declaration)
	{
		string args = declaration.Arguments?.Count > 0 ? $"({string.Join(", ", declaration.Arguments)})" : string.Empty;
		return $"{declaration.Name}{args}";
	}
	
}


