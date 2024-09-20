// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
public sealed class LazyObjectPtr<T> : LazyObjectPtrBase, IConjugate<LazyObjectPtr<T>> where T : UnrealObject
{
	
	public static LazyObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public LazyObjectPtr() : base(typeof(T)){}
	public LazyObjectPtr(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
	public LazyObjectPtr(UnrealObject? obj) : this()
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
	
	public bool IsNull => this.ZCall("ex://LazyObject.IsNull", false)[-1].Bool;
	
}


