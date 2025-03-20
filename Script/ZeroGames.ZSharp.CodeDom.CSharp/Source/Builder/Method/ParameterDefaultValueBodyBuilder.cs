// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class ParameterDefaultValueBodyBuilder(params ParameterDeclaration[]? parameters)
{

	public Block Build() => string.Join(Environment.NewLine, Parameters.Select(BuildAssignStatement).Where(content => !string.IsNullOrWhiteSpace(content)));

	private string BuildAssignStatement(ParameterDeclaration parameter)
	{
		string assignOperator = parameter.Type.TypeName.EndsWith("?") ? "??=" : "=";
		return !string.IsNullOrWhiteSpace(parameter.DefaultValue.Body.Content) ? $"{parameter.Name} {assignOperator} {parameter.DefaultValue.Body};" : string.Empty;
	}

	public ParameterDeclaration[] Parameters { get; } = parameters ?? [];

}


