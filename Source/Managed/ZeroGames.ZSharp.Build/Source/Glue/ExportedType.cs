﻿// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core;

namespace ZeroGames.ZSharp.Build.Glue;

public abstract class ExportedType : ExportedMember
{
	public required string Module { get; set; }
	public EZCallBufferSlotType SlotType { get; set; }
	public string Assembly { get; set; } = null!;
	
	public string Namespace => $"{Assembly}.{Module}";
}

