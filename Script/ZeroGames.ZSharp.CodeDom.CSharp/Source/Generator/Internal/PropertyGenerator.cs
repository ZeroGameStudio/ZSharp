// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.CodeDom.CSharp;

internal class PropertyGenerator
{

	public string Generate(PropertyDefinition definition)
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
		string propertyDecl = string.Join(" ", decls.Where(decl => !string.IsNullOrWhiteSpace(decl)));
		sb.Append(propertyDecl);

		bool isAbstraction = definition is { HasGetter: true, Getter: null } || definition is { HasSetter: true, Setter: null };
		if (isAbstraction)
		{
			sb.Append(" { ");
			if (definition.HasGetter)
			{
				sb.Append("get; ");
			}

			if (definition.HasSetter)
			{
				sb.Append("set; ");
			}

			sb.Append("}");
		}
		else
		{
			List<string> accessors = new();
			if (definition.HasGetter)
			{
				accessors.Add($"get{_bodyGenerator.Generate(definition.Getter!.Value, false)}");
			}
			
			if (definition.HasSetter)
			{
				accessors.Add($"set{_bodyGenerator.Generate(definition.Setter!.Value, false)}");
			}
			
			string accessorBody = string.Join(Environment.NewLine, accessors);
			string body = 
$@"
{{
{accessorBody.Indent()}
}}";

			sb.Append(body);
		}

		return sb.ToString();
	}

	private readonly AttributeListGenerator _attributeListGenerator = new();
	private MethodBodyGenerator _bodyGenerator = new();
	
}