// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

[StructLayout(LayoutKind.Sequential)]
public unsafe struct InteropRuntimeTypeLocator
{
	public char* AssemblyName;
	public char* TypeName;
	public InteropRuntimeTypeLocator* TypeParameters;
	public int32 NumTypeParameters;
}


