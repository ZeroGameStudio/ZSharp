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

		var parameters = definition.Signature.Parameters is not null ? definition.Signature.Parameters.Select(p => $"{p.GetKindText()}{p.Type.TypeName} {p.Name}") : [];
		string parameterList = string.Join(", ", parameters);
		
		string methodDecl = $"{declList}({parameterList})";
		sb.Append(methodDecl);

		if (definition.IsPureVirtual)
		{
			sb.Append(" => throw new NotImplementedException();");
		}
		else if (definition.Body is not null)
		{
			string body = _bodyGenerator.Generate(definition.Body.Value);
			sb.AppendLine();
			sb.Append(body);
		}
		else
		{
			sb.Append(';');
		}

		return sb.ToString();
	}

	private readonly AttributeListGenerator _attributeListGenerator = new();
	private MethodBodyGenerator _bodyGenerator = new();

}


