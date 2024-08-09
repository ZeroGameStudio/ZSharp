// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(32)]
public abstract class UnrealSetBase : PlainExportedObjectBase
{
	
	protected unsafe UnrealSetBase(Type elementType)
	{
		_elementType = elementType;
		ValidateElementType();
		
		SetupUserdata(out var userdata);
		Userdata* pUserdata = &userdata;
		Unmanaged = GetOwningAlc().BuildConjugate(this, (IntPtr)pUserdata);
	}
	
	protected UnrealSetBase(Type elementType, IntPtr unmanaged) : base(unmanaged)
	{
		_elementType = elementType;
		ValidateElementType();
	}
	
	private void ValidateElementType()
	{
		if (!ContainerHelper.CanBeKey(_elementType))
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


