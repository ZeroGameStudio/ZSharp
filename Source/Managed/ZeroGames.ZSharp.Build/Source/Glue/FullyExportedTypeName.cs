// Copyright Zero Games. All Rights Reserved.

using System.Text;
using System.Text.Json.Serialization;

namespace ZeroGames.ZSharp.Build.Glue;

public readonly struct SingleFullyExportedTypeName
{
	
	public required string Namespace { get; init; }
	public required string Name { get; init; }
	[JsonPropertyName("bNullable")] public required bool IsNullable { get; init; }

	public string ToString(bool nullable)
	{
		return Name + (IsNullable && nullable ? "?" : string.Empty);
	}

	public override string ToString() => ToString(true);
	
}

public readonly struct SimpleFullyExportedTypeName
{
	
	public required string Namespace { get; init; }
	public required string Name { get; init; }
	[JsonPropertyName("bNullable")] public required bool IsNullable { get; init; }
	public SingleFullyExportedTypeName Inner { get; init; }

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

		if (generic)
		{
			res.Append('>');
		}

		if (IsNullable && nullable)
		{
			res.Append('?');
		}

		return res.ToString();
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

		if (IsNullable && nullable)
		{
			res.Append('?');
		}

		return res.ToString();
	}

	public override string ToString() => ToString(true);
	
}


