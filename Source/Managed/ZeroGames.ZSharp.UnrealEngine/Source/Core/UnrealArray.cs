// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(31)]
public class UnrealArray<T> : PlainExportedObjectBase
{

	public unsafe UnrealArray()
	{
		ValidateElementType();
		
		SetupUserdata(out var userdata);
		Userdata* pUserdata = &userdata;
		Unmanaged = GetOwningAlc().BuildConjugate(this, (IntPtr)pUserdata);
	}

	public unsafe void InsertAt(int32 index)
	{
		IMasterAssemblyLoadContext alc = GetOwningAlc();
		const int32 numSlots = 2;
		ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
		{
			ZCallBufferSlot.FromConjugate(this),
			ZCallBufferSlot.FromInt32(index),
		};
		ZCallBuffer buffer = new(slots, numSlots);
		ZCallHandle handle = alc.GetZCallHandle("ex://Array.InsertAt");
		alc.ZCall(handle, &buffer);
	}

	public unsafe T this[int32 index]
	{
		get
		{
			IMasterAssemblyLoadContext alc = GetOwningAlc();
			const int32 numSlots = 3;
			ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
			{
				ZCallBufferSlot.FromConjugate(this),
				ZCallBufferSlot.FromInt32(index),
				ZCallBufferSlot.FromInt32(default),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("ex://Array.Get");
			alc.ZCall(handle, &buffer);

			object res = slots[2].ReadInt32();
			return (T)res;
		}
		set
		{
			IMasterAssemblyLoadContext alc = GetOwningAlc();
			const int32 numSlots = 3;
			ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
			{
				ZCallBufferSlot.FromConjugate(this),
				ZCallBufferSlot.FromInt32(index),
				ZCallBufferSlot.FromInt32((int32)(value as object)),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("ex://Array.Set");
			alc.ZCall(handle, &buffer);
		}
	}

	public unsafe int32 Count
	{
		get
		{
			IMasterAssemblyLoadContext alc = GetOwningAlc();
			const int32 numSlots = 2;
			ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
			{
				ZCallBufferSlot.FromConjugate(this),
				ZCallBufferSlot.FromInt32(default),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("ex://Array.Num");
			alc.ZCall(handle, &buffer);

			return slots[1].ReadInt32();
		}
	}

	private void ValidateElementType()
	{
		Type t = typeof(T);
		if (t.IsByRef || t.IsByRefLike)
		{
			throw new NotSupportedException();
		}

		if (t.IsPointer || t.IsFunctionPointer || typeof(T).IsUnmanagedFunctionPointer)
		{
			throw new NotSupportedException();
		}

		if (!(t.IsPrimitive || t.IsSubclassOf(typeof(UnrealObjectBase)) || t.IsSubclassOf(typeof(UnrealStructBase))))
		{
			throw new NotSupportedException();
		}
	}

	private static void SetupUserdata(out Userdata userdata)
	{
		userdata = new();
		
		Type t = typeof(T);
		if (IntrinsicTypeIds.STypeMap.TryGetValue(t, out var id))
		{
			userdata.ElementProperty.Descriptor = id;
		}
		else if (t.IsSubclassOf(typeof(UnrealObjectBase)))
		{
			userdata.ElementProperty.Descriptor = ((UnrealClass)t.GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!).Unmanaged;
		}
		else if (t.IsSubclassOf(typeof(UnrealStructBase)))
		{
			userdata.ElementProperty.Descriptor = ((UnrealScriptStruct)t.GetProperty(nameof(IStaticStruct.SStaticStruct))!.GetValue(null)!).Unmanaged;
		}
	}

	[StructLayout(LayoutKind.Sequential)]
	private struct Userdata
	{
		public SimplePropertyDesc ElementProperty;
	}
	
}


