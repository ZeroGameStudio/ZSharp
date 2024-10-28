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

		UntypedObject = obj;
	}
	
	public T? Load() => (T?)this.ZCall(MasterAlcCache.Instance, "ex://SoftObject.Load", [null])[-1].Object;
	
	public T? Object
	{
		get => (T?)UntypedObject;
		set => UntypedObject = value;
	}
	
	public bool IsNull => this.ZCall(MasterAlcCache.Instance, "ex://SoftObject.IsNull", false)[-1].Bool;
	
}


