// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class UnrealObjectWrapperBase : PlainExportedObjectBase
{

	public bool IsValid => _Object is not null;
	
	protected UnrealObjectWrapperBase(Type objectType, bool allowObject, bool allowInterface)
	{
		_objectType = objectType;
		_allowObject = allowObject;
		_allowInterface = allowInterface;
		ValidateElementType();
		
		Unmanaged = GetOwningAlc().BuildConjugate(this, UnrealObjectGlobals.GetClass(_objectType).Unmanaged);
	}

	protected UnrealObjectWrapperBase(Type objectType, bool allowObject, bool allowInterface, IntPtr unmanaged) : base(unmanaged)
	{
		_objectType = objectType;
		_allowObject = allowObject;
		_allowInterface = allowInterface;
		ValidateElementType();
	}
	
	protected abstract string _ZCallClassName { get; }
	
	protected UnrealObject? _Object
	{
		get => this.ZCall($"ex://{_ZCallClassName}.Get", [ null ])[-1].ReadConjugate<UnrealObject>();
		set => this.ZCall($"ex://{_ZCallClassName}.Set", value);
	}
	
	private void ValidateElementType()
	{
		bool objectMatch = _objectType.IsAssignableTo(typeof(UnrealObjectBase)) && _allowObject;
		bool interfaceMatch = _objectType.IsAssignableTo(typeof(IUnrealInterface)) && _allowInterface;
		if (!objectMatch && !interfaceMatch)
		{
			throw new NotSupportedException();
		}
	}

	private Type _objectType;
	private bool _allowObject;
	private bool _allowInterface;

}