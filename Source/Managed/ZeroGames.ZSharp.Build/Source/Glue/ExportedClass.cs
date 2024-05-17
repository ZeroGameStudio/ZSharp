// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core;

namespace ZeroGames.ZSharp.Build.Glue;

public class ExportedClass : ExportedType
{
	public ExportedClass() => SlotType = EZCallBufferSlotType.Conjugate;
	public required string BaseType { get; set; }
}


