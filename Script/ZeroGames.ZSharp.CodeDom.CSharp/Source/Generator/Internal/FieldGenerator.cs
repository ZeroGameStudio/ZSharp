// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class FieldGenerator
{

	public string Generate(FieldDefinition definition)
	{
		StringBuilder sb = new();
		
		if (definition.Attributes.Count > 0)
		{
			sb.AppendLine(string.Join(Environment.NewLine, definition.Attributes.Select(_attributeListGenerator.Generate)));
		}
		
		string visibility = definition.VisibilityText;
		string modifiers = definition.ModifiersText;
		string type = definition.Type.TypeName;
		string name = definition.Name;
		string[] decls = [ visibility, modifiers, type, name ];
		string declList = string.Join(" ", decls.Where(decl => !string.IsNullOrWhiteSpace(decl)));

		sb.Append(declList);
		sb.Append(';');

		return sb.ToString();
	}
	
	private readonly AttributeListGenerator _attributeListGenerator = new();
	
}


