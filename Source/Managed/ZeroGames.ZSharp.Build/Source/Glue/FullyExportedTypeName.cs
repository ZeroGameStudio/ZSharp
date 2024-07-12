// Copyright Zero Games. All Rights Reserved.

using System.Text;
using System.Text.Json.Serialization;

namespace ZeroGames.ZSharp.Build.Glue;

public readonly struct SimpleFullyExportedTypeName
{
	
	public required string Namespace { get; init; }
	public required string Name { get; init; }
	[JsonPropertyName("bNullable")] public required bool Nullable { get; init; }

	public string ToString(bool nullable)
	{
		return Name + (Nullable && nullable ? "?" : string.Empty);
	}

	public override string ToString() => ToString(true);
	
}

public readonly struct FullyExportedTypeName
{
	
	public required string Namespace { get; init; }
	public required string Name { get; init; }
	[JsonPropertyName("bNullable")] public required bool Nullable { get; init; }
	public SimpleFullyExportedTypeName Inner { get; init; }
	public SimpleFullyExportedTypeName Outer { get; init; }

	public string ToString(bool nullable)
	{
		StringBuilder res = new();
		res.Append(Name);
		bool generic = false;
		if (!string.IsNullOrWhiteSpace(Inner.Name))
		{
			generic = true;
			res.Append('<').Append(Inner.ToString(nullable));
		}

		if (!string.IsNullOrWhiteSpace(Outer.Name))
		{
			generic = true;
			res.Append(", ").Append(Outer.ToString(nullable));
		}

		if (generic)
		{
			res.Append('>');
		}

		if (Nullable && nullable)
		{
			res.Append('?');
		}

		return res.ToString();
	}

	public override string ToString() => ToString(true);
	
}


