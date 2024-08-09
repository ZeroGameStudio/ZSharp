// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(33)]
public abstract class UnrealMapBase : PlainExportedObjectBase
{
	
	protected unsafe UnrealMapBase(Type keyType, Type valueType)
	{
		_keyType = keyType;
		_valueType = valueType;
		ValidateElementType();
		
		SetupUserdata(out var userdata);
		Userdata* pUserdata = &userdata;
		Unmanaged = GetOwningAlc().BuildConjugate(this, (IntPtr)pUserdata);
	}
	
	protected UnrealMapBase(Type keyType, Type valueType, IntPtr unmanaged) : base(unmanaged)
	{
		_keyType = keyType;
		_valueType = valueType;
		ValidateElementType();
	}
	
	private void ValidateElementType()
	{
		if (!ContainerHelper.CanBeKey(_keyType) || !ContainerHelper.CanBeValue(_valueType))
		{
			throw new ArgumentOutOfRangeException();
		}
	}
	
	private void SetupUserdata(out Userdata userdata)
	{
		userdata = default;
		if (!ContainerHelper.TryGetPropertyDesc(_keyType, out userdata.KeyProperty) || !ContainerHelper.TryGetPropertyDesc(_valueType, out userdata.ValueProperty))
		{
			throw new ArgumentOutOfRangeException();
		}
	}

	private readonly Type _keyType;
	private readonly Type _valueType;
	
	[StructLayout(LayoutKind.Sequential)]
	private struct Userdata
	{
		public PropertyDesc KeyProperty;
		public PropertyDesc ValueProperty;
	}
	
}


