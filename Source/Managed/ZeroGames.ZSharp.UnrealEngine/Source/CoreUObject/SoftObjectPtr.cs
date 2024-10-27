// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
public sealed class SoftObjectPtr<T> : SoftObjectPtrBase, IConjugate<SoftObjectPtr<T>> where T : UnrealObject
{
	
	public static SoftObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public SoftObjectPtr() : base(typeof(T)){}
	public SoftObjectPtr(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
	public SoftObjectPtr(T? obj) : this()
	{
		if (obj is null)
		{
			return;
		}

		_Object = obj;
	}
	
	public T? Load() => (T?)this.ZCall("ex://SoftObject.Load", [null])[-1].Object;
	
	public T? Object
	{
		get => (T?)_Object;
		set => _Object = value;
	}
	
	public bool IsNull => this.ZCall("ex://SoftObject.IsNull", false)[-1].Bool;
	
}


