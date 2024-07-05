// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine;

[StructLayout(LayoutKind.Sequential)]
public struct SimplePropertyDesc
{
	public IntPtr Descriptor;
	public IntPtr Metadata;
}

[StructLayout(LayoutKind.Sequential)]
public struct PropertyDesc
{
	public IntPtr Descriptor;
	public SimplePropertyDesc InnerProperty;
	public SimplePropertyDesc OuterProperty;
}


