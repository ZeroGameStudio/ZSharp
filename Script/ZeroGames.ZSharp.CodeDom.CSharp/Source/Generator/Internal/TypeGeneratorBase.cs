// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.CodeDom.CSharp;

internal abstract class TypeGeneratorBase<TDefinition> where TDefinition : TypeDefinitionBase
{

	public string Generate(TDefinition definition)
	{
		StringBuilder sb = new();

		if (definition.Attributes.Count > 0)
		{
			sb.AppendLine(string.Join(Environment.NewLine, definition.Attributes.Select(_attributeListGenerator.Generate)));
		}

		string visibility = definition.GetVisibilityText();
		string modifiers = definition.GetModifiersText();
		string kind = GetTypeKindText(definition);
		string name = definition.Name;
		string baseList = definition.BaseTypes.Count > 0 ? $": {string.Join(", ", definition.BaseTypes)}" : string.Empty;
		string[] decls = [ visibility, modifiers, kind, name, baseList ];
		
		string typeDecl = $"{string.Join(" ", decls.Where(decl => !string.IsNullOrWhiteSpace(decl)))}";
		sb.Append(typeDecl);
		
		string typeBody = GenerateBody(definition);
		if (!string.IsNullOrWhiteSpace(typeBody))
		{
			sb.Append(
$@"
{{
{typeBody.Indent()}
}}");
		}
		else
		{
			sb.Append(';');
		}

		return sb.ToString();
	}

	protected abstract string GenerateBody(TDefinition definition);

	private string GetTypeKindText(TypeDefinitionBase definition) => definition.Kind switch
	{
		ETypeKind.Class => "class",
		ETypeKind.Interface => "interface",
		ETypeKind.Enum => "enum",
		_ => throw new NotSupportedException()
	};

	private readonly AttributeListGenerator _attributeListGenerator = new();

}


