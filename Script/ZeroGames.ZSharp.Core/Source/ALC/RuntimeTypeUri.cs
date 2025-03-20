// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public readonly struct RuntimeTypeUri(string? uri)
{
	public string? Uri { get; } = uri;

	internal unsafe RuntimeTypeUri(InteropRuntimeTypeUri uri) : this(new string(uri._uri)){}
}


