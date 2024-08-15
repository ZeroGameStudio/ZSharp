// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public sealed class SubclassOf<T> : SubclassOfBase, IConjugate<SubclassOf<T>> where T : IUnrealObject
{
	
	public static SubclassOf<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public SubclassOf() : base(typeof(T)){}
	public SubclassOf(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
	public SubclassOf(UnrealClass? cls) : this()
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
	
}


