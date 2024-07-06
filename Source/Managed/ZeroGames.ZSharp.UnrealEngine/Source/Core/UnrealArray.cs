// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(31)]
public abstract class UnrealArray : PlainExportedObjectBase
{
	
	protected unsafe UnrealArray(Type elementType)
	{
		_elementType = elementType;
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
	
	public unsafe object this[int32 index]
	{
		get
		{
			IMasterAssemblyLoadContext alc = GetOwningAlc();
			const int32 numSlots = 3;
			ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
			{
				ZCallBufferSlot.FromConjugate(this),
				ZCallBufferSlot.FromInt32(index),
				ZCallBufferSlot.FromType(_elementType),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("ex://Array.Get");
			alc.ZCall(handle, &buffer);

			return slots[2].Object!;
		}
		set
		{
			IMasterAssemblyLoadContext alc = GetOwningAlc();
			const int32 numSlots = 3;
			ZCallBufferSlot* slots = stackalloc ZCallBufferSlot[numSlots]
			{
				ZCallBufferSlot.FromConjugate(this),
				ZCallBufferSlot.FromInt32(index),
				ZCallBufferSlot.FromObject(value),
			};
			ZCallBuffer buffer = new(slots, numSlots);
			ZCallHandle handle = alc.GetZCallHandle("ex://Array.Set");
			alc.ZCall(handle, &buffer);
		}
	}
	
	private void ValidateElementType()
	{
		if (_elementType.IsByRef || _elementType.IsByRefLike)
		{
			throw new NotSupportedException();
		}

		if (_elementType.IsPointer || _elementType.IsFunctionPointer || _elementType.IsUnmanagedFunctionPointer)
		{
			throw new NotSupportedException();
		}

		if (!(_elementType.IsPrimitive || _elementType.IsSubclassOf(typeof(UnrealObjectBase)) || _elementType.IsSubclassOf(typeof(UnrealStructBase))))
		{
			throw new NotSupportedException();
		}
	}
	
	private void SetupUserdata(out Userdata userdata)
	{
		userdata = new();
		
		if (IntrinsicTypeIds.STypeMap.TryGetValue(_elementType, out var id))
		{
			userdata.ElementProperty.Descriptor = id;
		}
		else if (_elementType.IsSubclassOf(typeof(UnrealObjectBase)))
		{
			userdata.ElementProperty.Descriptor = ((UnrealClass)_elementType.GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!).Unmanaged;
		}
		else if (_elementType.IsSubclassOf(typeof(UnrealStructBase)))
		{
			userdata.ElementProperty.Descriptor = ((UnrealScriptStruct)_elementType.GetProperty(nameof(IStaticStruct.SStaticStruct))!.GetValue(null)!).Unmanaged;
		}
	}

	private Type _elementType;
	
	[StructLayout(LayoutKind.Sequential)]
	private struct Userdata
	{
		public SimplePropertyDesc ElementProperty;
	}

}

public class UnrealArray<T> : UnrealArray
{

	public UnrealArray() : base(typeof(T)){}

	public new T this[int32 index]
	{
		get => (T)((UnrealArray)this)[index];
		set => ((UnrealArray)this)[index] = value!;
	}
	
}


