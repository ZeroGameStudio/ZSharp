// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

[StructLayout(LayoutKind.Sequential)]
public readonly struct InteropRuntimeTypeHandle
{
	
	public InteropRuntimeTypeHandle(){}
	
	public InteropRuntimeTypeHandle(Type? type)
	{
		if (type is not null)
		{
			_handle = RuntimeTypeHandle.ToIntPtr(type.TypeHandle);
		}
	}

	public Type? Type => Type.GetTypeFromHandle(RuntimeTypeHandle.FromIntPtr(_handle));
	
	private readonly IntPtr _handle;
	
}


