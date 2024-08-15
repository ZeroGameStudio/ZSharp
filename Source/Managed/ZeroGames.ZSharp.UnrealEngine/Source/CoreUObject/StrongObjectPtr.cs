// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public sealed class StrongObjectPtr<T> : StrongObjectPtrBase, IConjugate<StrongObjectPtr<T>> where T : UnrealObject
{
	
	public static StrongObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public StrongObjectPtr() : base(typeof(T)){}
	public StrongObjectPtr(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
	public StrongObjectPtr(UnrealObject? obj) : this()
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
	
}


