// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(32)]
public abstract class UnrealSetBase : PlainExportedObjectBase
{

	public void Clear() => this.ZCall("ex://Set.Clear");

	public int32 Count => this.ZCall("ex://Set.Num", 0)[-1].Int32;
	
	protected unsafe UnrealSetBase(Type elementType)
	{
		_elementType = elementType;
		ValidateElementType();
		
		SetupUserdata(out var userdata);
		Userdata* pUserdata = &userdata;
		Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, (IntPtr)pUserdata);
	}
	
	protected UnrealSetBase(Type elementType, IntPtr unmanaged) : base(unmanaged)
	{
		_elementType = elementType;
		ValidateElementType();
	}

	protected void Add(object? value) => this.ZCall("ex://Set.Add", value);
	protected void Remove(object? value) => this.ZCall("ex://Set.Remove", value);
	protected bool Contains(object? value) => this.ZCall("ex://Set.Contains", value, false)[-1].Bool;
	
	[Conditional("ASSERTION_CHECK")]
	private void ValidateElementType()
	{
		check(ContainerHelper.CanBeKey(_elementType));
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


