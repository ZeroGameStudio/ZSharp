// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(34)]
public abstract class UnrealOptionalBase : PlainExportedObjectBase
{
	
	public void Reset() => this.ZCall("ex://Optional.Reset");

	public bool IsSet => this.ZCall("ex://Optional.IsSet", false)[-1].Bool;
	
	protected unsafe UnrealOptionalBase(Type elementType)
	{
		_elementType = elementType;
		ValidateElementType();
		
		SetupUserdata(out var userdata);
		Userdata* pUserdata = &userdata;
		Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, (IntPtr)pUserdata);
	}
	
	protected UnrealOptionalBase(Type elementType, IntPtr unmanaged) : base(unmanaged)
	{
		_elementType = elementType;
		ValidateElementType();
	}

	protected bool Get([NotNullWhen(true)] out object? value)
	{
		DynamicZCallResult res = this.ZCall("ex://Optional.Get", _elementType);
		value = res[1].Object;

		return res[-1].Bool;
	}
	protected void Set(object? value) => this.ZCall("ex://Optional.Set", value);
	
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


