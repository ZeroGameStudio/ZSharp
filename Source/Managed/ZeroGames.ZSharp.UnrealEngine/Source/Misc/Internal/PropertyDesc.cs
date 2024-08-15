// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine;

[StructLayout(LayoutKind.Sequential)]
internal struct PropertyDesc
{
	public IntPtr Descriptor;
	public IntPtr Metadata;
}


