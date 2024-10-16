// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.CodeDom.CSharp;

internal class MethodGenerator
{

	public string Generate(MethodDefinition definition)
	{
		StringBuilder sb = new();
		
		string visibility = definition.GetVisibilityText();
		string modifiers = definition.GetModifiersText();
		string kind = definition.IsDelegate ? "delegate" : string.Empty;
		string returnType = definition.ReturnType ?? "void";
		string name = definition.Name;
		string[] decls = [ visibility, modifiers, kind, returnType, name ];
		string declList = string.Join(" ", decls.Where(decl => !string.IsNullOrWhiteSpace(decl)));

		var parameters = definition.Parameters is not null ? definition.Parameters.Value.Declarations.Select(p => $"{p.GetKindText()}{p.Type} {p.Name}") : [];
		string parameterList = string.Join(", ", parameters);
		
		string methodDecl = $"{declList}({parameterList})";
		sb.Append(methodDecl);
		
		if (definition.Body is not null)
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

	private MethodBodyGenerator _bodyGenerator = new();

}


