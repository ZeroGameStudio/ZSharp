﻿// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class UnrealObjectWrapperBase : PlainExportedObjectBase
{

	public bool IsValid => UntypedObject is not null;
	
	protected UnrealObjectWrapperBase(Type objectType, bool allowObject, bool allowInterface)
	{
		_objectType = objectType;
		_allowObject = allowObject;
		_allowInterface = allowInterface;
		ValidateElementType();
		
		Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, GetStaticClass(_objectType).Unmanaged);
	}

	protected UnrealObjectWrapperBase(Type objectType, bool allowObject, bool allowInterface, IntPtr unmanaged) : base(unmanaged)
	{
		_objectType = objectType;
		_allowObject = allowObject;
		_allowInterface = allowInterface;
		ValidateElementType();
	}
	
	protected abstract string ZCallClassName { get; }
	
	protected UnrealObject? UntypedObject
	{
		get => this.ZCall(MasterAlcCache.Instance, $"ex://{ZCallClassName}.Get", [ null ])[-1].ReadConjugate<UnrealObject>();
		set => this.ZCall(MasterAlcCache.Instance, $"ex://{ZCallClassName}.Set", value);
	}
	
	[Conditional("ASSERTION_CHECK")]
	private void ValidateElementType()
	{
		bool objectMatch = _objectType.IsAssignableTo(typeof(UnrealObjectBase)) && _allowObject;
		bool interfaceMatch = _objectType.IsAssignableTo(typeof(IUnrealInterface)) && _allowInterface;
		check(objectMatch || interfaceMatch);
	}

	private readonly Type _objectType;
	private readonly bool _allowObject;
	private readonly bool _allowInterface;

}