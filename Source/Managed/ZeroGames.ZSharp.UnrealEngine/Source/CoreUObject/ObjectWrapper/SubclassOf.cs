﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
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
	
}

