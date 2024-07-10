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

	protected UnrealArray(Type elementType, IntPtr unmanaged) : base(unmanaged)
	{
		_elementType = elementType;
		ValidateElementType();
	}
	
	public void InsertAt(int32 index) => this.ZCall("ex://Array.InsertAt", index);

	public int32 Count => this.ZCall("ex://Array.Num", 0)[1].Int32;
	
	public object? this[int32 index]
	{
		get => this.ZCall("ex://Array.Get", index, _elementType)[2].Object;
		set => this.ZCall("ex://Array.Set", index, value);
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
		else if (UnrealEnum.IsUnrealEnumType(_elementType))
		{
			userdata.ElementProperty.Descriptor = UnrealEnum.GetStaticEnum(_elementType).Unmanaged;
		}
	}

	private readonly Type _elementType;
	
	[StructLayout(LayoutKind.Sequential)]
	private struct Userdata
	{
		public SimplePropertyDesc ElementProperty;
	}

}

public class UnrealArray<T> : UnrealArray, IConjugate<UnrealArray<T>>
{

	public static UnrealArray<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public UnrealArray() : base(typeof(T)){}
	public UnrealArray(IntPtr unmanaged) : base(typeof(T), unmanaged){}

	public new T? this[int32 index]
	{
		get => (T?)((UnrealArray)this)[index];
		set => ((UnrealArray)this)[index] = value!;
	}
}


