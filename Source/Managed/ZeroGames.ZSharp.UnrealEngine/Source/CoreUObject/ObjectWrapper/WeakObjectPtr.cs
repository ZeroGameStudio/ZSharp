// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
public sealed class WeakObjectPtr<T> : WeakObjectPtrBase, IConjugate<WeakObjectPtr<T>> where T : UnrealObject
{
	
	public static WeakObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public WeakObjectPtr() : base(typeof(T)){}
	public WeakObjectPtr(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
	public WeakObjectPtr(T? obj) : this()
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
	
	public bool IsNull => this.ZCall(MasterAlcCache.Instance, "ex://WeakObject.IsNull", false)[-1].Bool;
	
}


