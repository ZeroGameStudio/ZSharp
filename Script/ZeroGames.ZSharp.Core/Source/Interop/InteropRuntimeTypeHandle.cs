// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

[StructLayout(LayoutKind.Sequential)]
public readonly struct InteropRuntimeTypeHandle : IEquatable<InteropRuntimeTypeHandle>
{

	public InteropRuntimeTypeHandle(){}
	
	public InteropRuntimeTypeHandle(Type? type)
	{
		if (type is not null)
		{
			Handle = RuntimeTypeHandle.ToIntPtr(type.TypeHandle);
		}
	}
	
	public override bool Equals(object? obj)
	{
		return obj is InteropRuntimeTypeHandle other && Equals(other);
	}

	public override int32 GetHashCode()
	{
		return Handle.GetHashCode();
	}
	
	public bool Equals(InteropRuntimeTypeHandle other)
	{
		return Handle == other.Handle;
	}

	public IntPtr Handle { get; }
	
}


