// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class FieldGenerator
{

	public string Generate(FieldDefinition definition)
	{
		string visibility = definition.GetVisibilityText();
		string modifiers = definition.GetModifiersText();
		string type = definition.Type.TypeName;
		string name = definition.Name;
		string[] decls = [ visibility, modifiers, type, name ];
		string declList = string.Join(" ", decls.Where(decl => !string.IsNullOrWhiteSpace(decl)));
		return declList + ';';
	}
	
}


