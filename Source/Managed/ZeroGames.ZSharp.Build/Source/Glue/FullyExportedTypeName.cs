// Copyright Zero Games. All Rights Reserved.

using System.Text;
using System.Text.Json.Serialization;

namespace ZeroGames.ZSharp.Build.Glue;

public readonly struct SimpleFullyExportedTypeName
{
	
	public required string Namespace { get; init; }
	public required string Name { get; init; }
	[JsonPropertyName("bNullable")] public required bool IsNullable { get; init; }

	public string ToString(bool isNullable)
	{
		return Name + (IsNullable && isNullable ? "?" : string.Empty);
	}

	public override string ToString() => ToString(true);
	
}

public readonly struct FullyExportedTypeName
{
	
	public required string Namespace { get; init; }
	public required string Name { get; init; }
	[JsonPropertyName("bNullable")] public required bool IsNullable { get; init; }
	public SimpleFullyExportedTypeName Inner { get; init; }
	public SimpleFullyExportedTypeName Outer { get; init; }

	public string ToString(bool isNullable)
	{
		StringBuilder res = new();
		res.Append(Name);
		bool isGeneric = false;
		if (!string.IsNullOrWhiteSpace(Inner.Name))
		{
			isGeneric = true;
			res.Append('<').Append(Inner.ToString(isNullable));
		}

		if (!string.IsNullOrWhiteSpace(Outer.Name))
		{
			isGeneric = true;
			res.Append(", ").Append(Outer.ToString(isNullable));
		}

		if (isGeneric)
		{
			res.Append('>');
		}

		if (IsNullable && isNullable)
		{
			res.Append('?');
		}

		return res.ToString();
	}

	public override string ToString() => ToString(true);
	
}


