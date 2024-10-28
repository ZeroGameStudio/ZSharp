// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(31)]
public abstract class UnrealArrayBase : PlainExportedObjectBase
{
	
	public void InsertAt(int32 index) => this.ZCall(MasterAlcCache.Instance, "ex://Array.InsertAt", index);
	public void RemoveAt(int32 index) => this.ZCall(MasterAlcCache.Instance, "ex://Array.RemoveAt", index);
	public void Clear() => this.ZCall(MasterAlcCache.Instance, "ex://Array.Clear");
	
	public int32 Count => this.ZCall(MasterAlcCache.Instance, "ex://Array.Num", 0)[-1].Int32;

	protected unsafe UnrealArrayBase(Type elementType)
	{
		_elementType = elementType;
		ValidateElementType();
		
		SetupUserdata(out var userdata);
		Userdata* pUserdata = &userdata;
		Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, (IntPtr)pUserdata);
	}

	protected UnrealArrayBase(Type elementType, IntPtr unmanaged) : base(unmanaged)
	{
		_elementType = elementType;
		ValidateElementType();
	}
	
	protected object? Get(int32 index) => this.ZCall(MasterAlcCache.Instance, "ex://Array.Get", index, _elementType)[-1].Object;
	protected void Set(int32 index, object? value) => this.ZCall(MasterAlcCache.Instance, "ex://Array.Set", index, value);
	
	[Conditional("ASSERTION_CHECK")]
	private void ValidateElementType()
	{
		check(ContainerHelper.CanBeValue(_elementType));
	}
	
	private void SetupUserdata(out Userdata userdata)
	{
		userdata = default;
		verify(ContainerHelper.TryGetPropertyDesc(_elementType, out userdata.ElementProperty));
	}

	private readonly Type _elementType;
	
	[StructLayout(LayoutKind.Sequential)]
	private struct Userdata
	{
		public PropertyDesc ElementProperty;
	}

}


