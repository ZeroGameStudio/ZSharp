// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

public abstract class ExportedType : ExportedMember
{
	public string Module { get; set; } = string.Empty;
	public EZCallBufferSlotType SlotType { get; set; } = EZCallBufferSlotType.Conjugate;
	public string Assembly { get; set; } = string.Empty;
	
	public string Namespace => $"{Assembly}.{Module}";
}


