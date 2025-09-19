// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class ZCallPropertyBuilder(EMemberVisibility visibility, EMemberModifiers modifiers, string name, string zcallName, int32 index, TypeReference type, bool needsUnsafeBlock, bool readable, bool writable, bool refGetter)
{
	
	public PropertyDefinition Build(bool abstraction) => new(Visibility, Name, Type, IsReadable, IsWritable)
	{
		Modifiers = Modifiers,
		Getter = !abstraction && IsReadable ? new ZCallMethodBodyBuilder(ZCallName, Type, IsRefGetter, NeedsUnsafeBlock,
			new ParameterDeclaration(EParameterKind.In, new("bool", null, false, false), string.Empty),
			new ParameterDeclaration(EParameterKind.In, new(nameof(int32), null, false, false), Index != 0 ? Index.ToString() : string.Empty))
			{
				BeforeReturnBlock = BeforeGetterReturnBlock,
			}.Build() : null,
		Setter = !abstraction && IsWritable ? new ZCallMethodBodyBuilder(ZCallName, null, false, NeedsUnsafeBlock,
			new ParameterDeclaration(EParameterKind.In, new("bool", null, false, false), "true"),
			new ParameterDeclaration(EParameterKind.In, new(nameof(int32), null, false, false), Index != 0 ? Index.ToString() : string.Empty),
			new ParameterDeclaration(EParameterKind.In, Type, "value"))
			{
				BeforeReturnBlock = BeforeSetterReturnBlock,
			}.Build() : null,
	};
	
	public EMemberVisibility Visibility { get; } = visibility;
	public EMemberModifiers Modifiers { get; } = modifiers;
	public string Name { get; } = name;
	public string ZCallName { get; } = zcallName;
	public int32 Index { get; } = index;
	public TypeReference Type { get; } = type;
	public bool NeedsUnsafeBlock { get; } = needsUnsafeBlock;
	public bool IsReadable { get; } = readable;
	public bool IsWritable { get; } = writable;
	public bool IsRefGetter { get; } = refGetter;
	
	public Block BeforeGetterReturnBlock { get; set; } = new();
	public Block BeforeSetterReturnBlock { get; set; } = new();
	
}


