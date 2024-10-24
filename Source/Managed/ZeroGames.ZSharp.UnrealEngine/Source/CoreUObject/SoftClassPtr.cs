﻿// Copyright Zero Games. All Rights Reserved.

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
		
		if (!cls.IsChildOf(UnrealObjectGlobals.GetClass<T>()))
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
	public SubclassOf<T> Load => new((UnrealClass?)this.ZCall("ex://SoftClass.Load", [null])[-1].Object);

}


