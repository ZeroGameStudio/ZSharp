// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class ZCallMethodBuilder(EMemberVisibility visibility, EMemberModifiers modifiers, string name, string zcallName, TypeReference? returnType, bool needsUnsafeBlock, params ParameterDeclaration[]? parameters)
{

	public MethodDefinition Build(bool abstraction) => new(Visibility, Name, ReturnType, Parameters?.ToArray())
	{
		Modifiers = Modifiers,
		Body = abstraction ? null : new ZCallMethodBodyBuilder(ZCallName, ReturnType, NeedsUnsafeBlock, Parameters?.ToArray())
		{
			IsStatic = Modifiers.HasFlag(EMemberModifiers.Static),
			IsVirtual = Modifiers.HasFlag(EMemberModifiers.Virtual),
		}.Build(),
	};
	
	public EMemberVisibility Visibility { get; } = visibility;
	public EMemberModifiers Modifiers { get; } = modifiers;
	public string Name { get; } = name;
	public string ZCallName { get; } = zcallName;
	public TypeReference? ReturnType { get; } = returnType;
	public bool NeedsUnsafeBlock { get; } = needsUnsafeBlock;
	public IReadOnlyList<ParameterDeclaration>? Parameters { get; } = parameters;

}


