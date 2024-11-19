// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
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
		
		if (!ensure(cls.IsChildOf(GetStaticClass<T>())))
		{
			return;
		}

		UntypedObject = cls;
	}
	
	public UnrealClass? Class
	{
		get => (UnrealClass?)UntypedObject;
		set => UntypedObject = value;
	}

	public bool IsNull => this.ZCall(MasterAlcCache.Instance, "ex://SoftClass.IsNull", false)[-1].Bool;
	public SubclassOf<T> Load => new((UnrealClass?)this.ZCall(MasterAlcCache.Instance, "ex://SoftClass.Load", [null])[-1].Object);

}


