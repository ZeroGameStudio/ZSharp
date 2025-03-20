// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.CodeDom.CSharp;

internal class MethodGenerator
{

	public string Generate(MethodDefinition definition)
	{
		StringBuilder sb = new();
		
		if (definition.Attributes.Count > 0)
		{
			sb.AppendLine(string.Join(Environment.NewLine, definition.Attributes.Select(_attributeListGenerator.Generate)));
		}
		
		string visibility = definition.GetVisibilityText();
		string modifiers = definition.GetModifiersText();
		string kind = definition.IsDelegate ? "delegate" : string.Empty;
		string returnType = definition.Signature.ReturnType?.TypeName ?? "void";
		string name = definition.Name;
		string[] decls = [ visibility, modifiers, kind, returnType, name ];
		string declList = string.Join(" ", decls.Where(decl => !string.IsNullOrWhiteSpace(decl)));

		var parameters = definition.Signature.Parameters is not null ? definition.Signature.Parameters.Select(GenerateParameterDecl) : [];
		string parameterList = string.Join(", ", parameters);
		
		string methodDecl = $"{declList}({parameterList})";
		sb.Append(methodDecl);

		if (definition.Body is not null)
		{
			sb.Append(_bodyGenerator.Generate(definition.Body.Value, definition.IsSingleLine));
		}
		else
		{
			sb.Append(';');
		}

		return sb.ToString();
	}

	private string GenerateParameterDecl(ParameterDeclaration parameter)
	{
		string defaultValueComment = !string.IsNullOrWhiteSpace(parameter.DefaultValue.Body.Content) && !parameter.DefaultValue.Body.Content.Contains('\n') ? $" /* {parameter.DefaultValue.Body} */" : string.Empty;
		string defaultValue = !string.IsNullOrWhiteSpace(parameter.DefaultValue.Signature.Content) ? $" = {parameter.DefaultValue.Signature.Content}{defaultValueComment}" : string.Empty;
		string attributes = parameter.Attributes is not null ? _attributeListGenerator.Generate(parameter.Attributes.Value) : string.Empty;
		if (!string.IsNullOrWhiteSpace(attributes))
		{
			attributes += ' ';
		}
		return $"{attributes}{parameter.GetKindText()}{parameter.Type.TypeName} {parameter.Name}{defaultValue}";
	}

	private readonly AttributeListGenerator _attributeListGenerator = new();
	private MethodBodyGenerator _bodyGenerator = new();

}


