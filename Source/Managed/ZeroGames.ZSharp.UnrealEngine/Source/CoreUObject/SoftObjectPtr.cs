// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public class SoftObjectPtr<T> : SoftObjectPtrBase, IConjugate<SoftObjectPtr<T>> where T : UnrealObject
{
	
	public static SoftObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public SoftObjectPtr() : base(typeof(T)){}
	public SoftObjectPtr(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
	public SoftObjectPtr(UnrealObject? obj) : this()
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
	
	public bool IsNull => this.ZCall("ex://SoftObject.IsNull", false)[-1].Bool;
	public T? Load => (T?)this.ZCall("ex://SoftObject.Load", [null])[-1].Object;
	
}


