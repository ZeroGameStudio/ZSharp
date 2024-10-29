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
			_handle = RuntimeTypeHandle.ToIntPtr(type.TypeHandle);
		}
	}
	
	public override bool Equals(object? obj)
	{
		return obj is InteropRuntimeTypeHandle other && Equals(other);
	}

	public override int32 GetHashCode()
	{
		return _handle.GetHashCode();
	}
	
	public bool Equals(InteropRuntimeTypeHandle other)
	{
		return _handle == other._handle;
	}

	public Type? Type => Type.GetTypeFromHandle(RuntimeTypeHandle.FromIntPtr(_handle));
	
	private readonly IntPtr _handle;
	
}


