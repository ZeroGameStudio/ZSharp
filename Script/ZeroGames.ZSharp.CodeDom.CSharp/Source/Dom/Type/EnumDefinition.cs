// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class EnumDefinition(EMemberVisibility visibility, string name) : TypeDefinitionBase(visibility, name)
{

	public readonly struct Member(string name, string value, string comment)
	{
		public string Name { get; } = name;
		public string Value { get; } = value;
		public string Comment { get; } = comment;
	}

	public void AddMember(string name, string value = "", string comment = "")
	{
		if (_members.Any(member => member.Name == name))
		{
			throw new InvalidOperationException();
		}

		_members.Add(new(name, value, comment));
	}

	public override ETypeKind Kind => ETypeKind.Enum;
	public IReadOnlyList<Member> Members => _members;

	private readonly List<Member> _members = new();

}


