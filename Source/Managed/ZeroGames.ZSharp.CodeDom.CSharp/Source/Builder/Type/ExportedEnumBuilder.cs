// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class ExportedEnumBuilder(string namespaceName, string typeName, string? unrealFieldPath) : ExportedTypeBuilderBase<EnumDefinition>(namespaceName, typeName, unrealFieldPath)
{

	public void AddMember(string name, string value = "", string comment = "") => _members.Add((name, value, comment));

	public bool IsFlags { get; set; }
	public string UnderlyingType { get; set; } = nameof(int32);
	
	protected override EnumDefinition AllocateTypeDefinition() => new(EMemberVisibility.Public, TypeName);

	protected override void BuildTypeDefinition(EnumDefinition definition)
	{
		AddBaseType(UnderlyingType);
		if (IsFlags)
		{
			AddAttributeBefore("Flags");
		}
		
		foreach (var member in _members)
		{
			definition.AddMember(member.Name, member.Value, member.Comment);
		}
	}

	protected override bool HasUnrealFieldPathConst => false;

	private readonly List<(string Name, string Value, string Comment)> _members = new();

}


