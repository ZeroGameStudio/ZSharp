// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

[StructLayout(LayoutKind.Sequential)]
public readonly unsafe struct InteropRuntimeTypeUri
{
	public readonly char* AssemblyName;
	public readonly char* TypeName;
	public readonly InteropRuntimeTypeUri* TypeParameters;
	public readonly int32 NumTypeParameters;
}


