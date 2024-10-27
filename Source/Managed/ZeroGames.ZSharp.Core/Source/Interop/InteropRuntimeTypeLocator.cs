// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

[StructLayout(LayoutKind.Sequential)]
public readonly unsafe struct InteropRuntimeTypeLocator
{
	public readonly char* AssemblyName;
	public readonly char* TypeName;
	public readonly InteropRuntimeTypeLocator* TypeParameters;
	public readonly int32 NumTypeParameters;
}


