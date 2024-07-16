// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class UnrealObjectWrapperBase : PlainExportedObjectBase
{
	
	protected UnrealObjectWrapperBase(Type objectType, bool allowObject, bool allowInterface)
	{
		_objectType = objectType;
		_allowObject = allowObject;
		_allowInterface = allowInterface;
		ValidateElementType();
		
		Unmanaged = GetOwningAlc().BuildConjugate(this, ((UnrealClass)_objectType.GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!).Unmanaged);
	}

	protected UnrealObjectWrapperBase(Type objectType, bool allowObject, bool allowInterface, IntPtr unmanaged) : base(unmanaged)
	{
		_objectType = objectType;
		_allowObject = allowObject;
		_allowInterface = allowInterface;
		ValidateElementType();
	}
	
	private void ValidateElementType()
	{
		bool objectMatch = _objectType.IsSubclassOf(typeof(UnrealObjectBase)) && _allowObject;
		bool interfaceMatch = _objectType.IsSubclassOf(typeof(IUnrealInterface)) && _allowInterface;
		if (!objectMatch && !interfaceMatch)
		{
			throw new NotSupportedException();
		}
	}

	private Type _objectType;
	private bool _allowObject;
	private bool _allowInterface;

}