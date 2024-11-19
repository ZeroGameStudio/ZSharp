// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
public sealed class LazyObjectPtr<T> : LazyObjectPtrBase, IConjugate<LazyObjectPtr<T>> where T : UnrealObject
{
	
	public static LazyObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public LazyObjectPtr() : base(typeof(T)){}
	public LazyObjectPtr(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
	public LazyObjectPtr(T? obj) : this()
	{
		if (obj is null)
		{
			return;
		}

		UntypedObject = obj;
	}
	
	public T? Object
	{
		get => (T?)UntypedObject;
		set => UntypedObject = value;
	}
	
	public bool IsNull => this.ZCall(MasterAlcCache.Instance, "ex://LazyObject.IsNull", false)[-1].Bool;
	
}


