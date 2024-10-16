// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.CodeDom.CSharp;

internal class EnumGenerator : TypeGeneratorBase<EnumDefinition>
{

	protected override string GenerateBody(EnumDefinition definition)
	{
		var members = definition.Members.Select(GenerateMember);
		return $"{string.Join(Environment.NewLine, members)}";
	}

	private string GenerateMember(EnumDefinition.Member member)
	{
		StringBuilder sb = new();
		if (!string.IsNullOrWhiteSpace(member.Comment))
		{
			sb.AppendLine(member.Comment);
		}

		string value = !string.IsNullOrWhiteSpace(member.Value) ? $" = {member.Value}" : string.Empty;
		sb.Append($"{member.Name}{value},");

		return sb.ToString();
	}

}


