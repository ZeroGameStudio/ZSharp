// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

[StructLayout(LayoutKind.Sequential)]
public readonly unsafe struct InteropRuntimeTypeUri
{
	internal readonly char* _uri;
}


