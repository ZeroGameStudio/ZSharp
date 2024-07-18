// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(31)]
public abstract class UnrealArrayBase : PlainExportedObjectBase
{
	
	public void InsertAt(int32 index) => this.ZCall("ex://Array.InsertAt", index);
	public void RemoveAt(int32 index) => this.ZCall("ex://Array.RemoveAt", index);
	
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
		if (_elementType.IsAssignableTo(typeof(UnrealObjectBase)) || _elementType.IsAssignableTo(typeof(UnrealStructBase)) || UnrealEnum.IsUnrealEnumType(_elementType))
		{
			return;
		}

		if (_elementType.GetGenericTypeDefinition() == typeof(Nullable<>))
		{
			throw new NotSupportedException("Nullable value type is not supported.");
		}
		
		if (!IntrinsicTypeIds.STypeMap.ContainsKey(_elementType) && (_elementType.BaseType is null || !IntrinsicTypeIds.STypeMap.ContainsKey(_elementType.BaseType)))
		{
			throw new NotSupportedException();
		}
	}
	
	private void SetupUserdata(out Userdata userdata)
	{
		userdata = new();

		IntPtr id;
		if (IntrinsicTypeIds.STypeMap.TryGetValue(_elementType, out id) || (_elementType.BaseType is not null && IntrinsicTypeIds.STypeMap.TryGetValue(_elementType.BaseType, out id)))
		{
			userdata.ElementProperty.Descriptor = id;
		}
		else if (_elementType.IsAssignableTo(typeof(UnrealObjectBase)))
		{
			userdata.ElementProperty.Descriptor = ((UnrealClass)_elementType.GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!).Unmanaged;
		}
		else if (_elementType.IsAssignableTo(typeof(UnrealStructBase)))
		{
			userdata.ElementProperty.Descriptor = ((UnrealScriptStruct)_elementType.GetProperty(nameof(IStaticStruct.SStaticStruct))!.GetValue(null)!).Unmanaged;
		}
		else if (UnrealEnum.IsUnrealEnumType(_elementType))
		{
			userdata.ElementProperty.Descriptor = UnrealEnum.GetUnrealEnum(_elementType).Unmanaged;
		}
	}

	private readonly Type _elementType;
	
	[StructLayout(LayoutKind.Sequential)]
	private struct Userdata
	{
		public PropertyDesc ElementProperty;
	}

}


