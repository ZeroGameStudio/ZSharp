// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class ZCallPropertyBuilder(EMemberVisibility visibility, EMemberModifiers modifiers, string name, string zcallName, int32 index, TypeReference type, bool readOnly)
{
	
	public PropertyDefinition Build(bool abstraction) => new(Visibility, Name, Type, true, !IsReadOnly)
	{
		Modifiers = Modifiers,
		// @TODO: Partial property
		Getter = new ZCallMethodBodyBuilder(ZCallName, Type,
			new ParameterDeclaration(EParameterKind.In, new("bool", null), string.Empty),
			new ParameterDeclaration(EParameterKind.In, new(nameof(int32), null), Index != 0 ? Index.ToString() : string.Empty)).Build(),
		Setter = !IsReadOnly ? new ZCallMethodBodyBuilder(ZCallName, null,
			new ParameterDeclaration(EParameterKind.In, new("bool", null), true.ToString()),
			new ParameterDeclaration(EParameterKind.In, new(nameof(int32), null), Index != 0 ? Index.ToString() : string.Empty),
			new ParameterDeclaration(EParameterKind.In, Type, "value")).Build() : null,
	};
	
	public EMemberVisibility Visibility { get; } = visibility;
	public EMemberModifiers Modifiers { get; } = modifiers;
	public string Name { get; } = name;
	public string ZCallName { get; } = zcallName;
	public int32 Index { get; } = index;
	public TypeReference Type { get; } = type;
	public bool IsReadOnly { get; } = readOnly;
	
}


