// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(31)]
public abstract class UnrealArrayBase : PlainExportedObjectBase
{
	
	public void InsertAt(int32 index) => this.ZCall("ex://Array.InsertAt", index);
	public void RemoveAt(int32 index) => this.ZCall("ex://Array.RemoveAt", index);
	public void Clear() => this.ZCall("ex://Array.Clear");
	
	public int32 Count => this.ZCall("ex://Array.Num", 0)[-1].Int32;

	protected unsafe UnrealArrayBase(Type elementType)
	{
		_elementType = elementType;
		ValidateElementType();
		
		SetupUserdata(out var userdata);
		Userdata* pUserdata = &userdata;
		Unmanaged = GetOwningAlc().BuildConjugate(this, (IntPtr)pUserdata);
	}

	protected UnrealArrayBase(Type elementType, IntPtr unmanaged) : base(unmanaged)
	{
		_elementType = elementType;
		ValidateElementType();
	}
	
	protected object? Get(int32 index) => this.ZCall("ex://Array.Get", index, _elementType)[-1].Object;
	protected void Set(int32 index, object? value) => this.ZCall("ex://Array.Set", index, value);
	
	private void ValidateElementType()
	{
		if (!ContainerHelper.CanBeValue(_elementType))
		{
			throw new ArgumentOutOfRangeException();
		}
	}
	
	private void SetupUserdata(out Userdata userdata)
	{
		userdata = default;
		if (!ContainerHelper.TryGetPropertyDesc(_elementType, out userdata.ElementProperty))
		{
			throw new ArgumentOutOfRangeException();
		}
	}

	private readonly Type _elementType;
	
	[StructLayout(LayoutKind.Sequential)]
	private struct Userdata
	{
		public PropertyDesc ElementProperty;
	}

}


