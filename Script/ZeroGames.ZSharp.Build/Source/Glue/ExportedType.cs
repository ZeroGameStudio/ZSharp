// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

public abstract class ExportedType : ExportedMember
{
	public required string Module { get; set; }
	public string? UnrealFieldPath { get; set; }
	
	public EZCallBufferSlotType SlotType { get; set; }
	public string Assembly { get; set; } = null!;
	
	public string Namespace => Assembly.Split('.').Last() == Module ? Assembly : $"{Assembly}.{Module}";
}


