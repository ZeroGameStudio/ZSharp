// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class InterfaceMethodBodyBuilder(string interfaceName, string name, TypeReference? returnType, params ParameterDeclaration[]? parameters)
{

	public MethodBody Build()
	{
		string parameterList = string.Join(", ", Parameters?.Select(p =>
		{
			string kind = p.Kind switch
			{
				EParameterKind.Out => "out ",
				EParameterKind.Ref => "ref ",
				_ => string.Empty
			};
			return $"{kind}{p.Name}";
		}) ?? []);
		string body = $"(({InterfaceName})this).{name}({parameterList});";
		return new(body);
	}

	public string InterfaceName { get; } = interfaceName;
	public string Name { get; } = name;
	public TypeReference? ReturnType { get; } = returnType;
	public IReadOnlyList<ParameterDeclaration>? Parameters { get; } = parameters;
	
}


