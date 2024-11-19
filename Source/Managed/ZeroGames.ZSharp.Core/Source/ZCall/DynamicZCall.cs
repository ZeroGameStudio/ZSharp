// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public readonly struct DynamicZCallResult(ZCallBufferSlot[] slots)
{

	public int32 NumSlots => _slots.Length;
	
	public ref readonly ZCallBufferSlot this[int32 index]
	{
		get
		{
			if (index < 0)
			{
				index = _slots.Length + index;
			}
			
			if (index < 0 || index >= NumSlots)
			{
				throw new IndexOutOfRangeException();
			}
			
			return ref _slots[index];
		}
	}

	private readonly ZCallBufferSlot[] _slots = slots;

}

public static class DynamicZCall
{

	public static DynamicZCallResult ZCall(IMasterAssemblyLoadContext alc, ZCallHandle handle, params ReadOnlySpan<object?> parameters) => InternalZCall(alc, handle, parameters);
	
	public static DynamicZCallResult ZCall(IMasterAssemblyLoadContext alc, string name, out ZCallHandle outHandle, params ReadOnlySpan<object?> parameters)
	{
		outHandle = alc.GetZCallHandle(name);
		return InternalZCall(alc, outHandle, parameters);
	}

	public static DynamicZCallResult ZCall(IMasterAssemblyLoadContext alc, string name, params ReadOnlySpan<object?> parameters) => ZCall(alc, name, out _, parameters);
	
	public static DynamicZCallResult ZCall(this IConjugate @this, IMasterAssemblyLoadContext alc, ZCallHandle handle, params ReadOnlySpan<object?> parameters) => @this.InternalZCall(alc, handle, parameters);
	
	public static DynamicZCallResult ZCall(this IConjugate @this, IMasterAssemblyLoadContext alc, string name, out ZCallHandle outHandle, params ReadOnlySpan<object?> parameters)
	{
		outHandle = alc.GetZCallHandle(name);
		return InternalZCall(@this, alc, outHandle, parameters);
	}

	public static DynamicZCallResult ZCall(this IConjugate @this, IMasterAssemblyLoadContext alc, string name, params ReadOnlySpan<object?> parameters) => @this.ZCall(alc, name, out _, parameters);
	
	private static unsafe DynamicZCallResult InternalZCall(IMasterAssemblyLoadContext alc, ZCallHandle handle, params ReadOnlySpan<object?> parameters)
	{
		ZCallBufferSlot[] slots = new ZCallBufferSlot[parameters.Length];
		for (int32 i = 0; i < slots.Length; ++i)
		{
			slots[i] = ZCallBufferSlot.FromObject(parameters[i]);
		}
		
		fixed (ZCallBufferSlot* pSlots = slots)
		{
			ZCallBuffer buffer = new(pSlots, slots.Length);
			alc.ZCall(handle, &buffer);
		}
		
		return new(slots);
	}
	
	private static unsafe DynamicZCallResult InternalZCall(this IConjugate @this, IMasterAssemblyLoadContext alc, ZCallHandle handle, params ReadOnlySpan<object?> parameters)
	{
		ZCallBufferSlot[] slots = new ZCallBufferSlot[parameters.Length + 1];
		slots[0] = ZCallBufferSlot.FromConjugate(@this);
		for (int32 i = 1; i < slots.Length; ++i)
		{
			slots[i] = ZCallBufferSlot.FromObject(parameters[i - 1]);
		}
		
		fixed (ZCallBufferSlot* pSlots = slots)
		{
			ZCallBuffer buffer = new(pSlots, slots.Length);
			alc.ZCall(handle, &buffer);
		}
		
		return new(slots);
	}
	
}


