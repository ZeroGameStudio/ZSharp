// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public sealed class WeakObjectPtr<T> : WeakObjectPtrBase, IConjugate<WeakObjectPtr<T>> where T : UnrealObject
{
	
	public static WeakObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public WeakObjectPtr() : base(typeof(T)){}
	public WeakObjectPtr(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
	public WeakObjectPtr(UnrealObject? obj) : this()
	{
		if (obj is null)
		{
			return;
		}

		if (!obj.IsA<T>())
		{
			throw new NotSupportedException();
		}

		_Object = obj;
	}
	
	public T? Object
	{
		get => (T?)_Object;
		set => _Object = value;
	}
	
	public bool IsNull => this.ZCall("ex://WeakObject.IsNull", false)[-1].Bool;
	
}


