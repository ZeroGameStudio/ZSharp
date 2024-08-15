// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public sealed class SoftClassPtr<T> : SoftClassPtrBase, IConjugate<SoftClassPtr<T>> where T : IUnrealObject
{
	
	public static SoftClassPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public SoftClassPtr() : base(typeof(T)){}
	public SoftClassPtr(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
	public SoftClassPtr(UnrealClass? cls) : this()
	{
		if (cls is null)
		{
			return;
		}
		
		if (!cls.IsChildOf((UnrealClass)typeof(T).GetProperty(nameof(IStaticClass.SStaticClass))!.GetValue(null)!))
		{
			throw new NotSupportedException();
		}

		_Object = cls;
	}
	
	public UnrealClass? Class
	{
		get => (UnrealClass?)_Object;
		set => _Object = value;
	}

	public bool IsNull => this.ZCall("ex://SoftClass.IsNull", false)[-1].Bool;
	public T? Load => (T?)this.ZCall("ex://SoftClass.Load", [null])[-1].Object;

}


