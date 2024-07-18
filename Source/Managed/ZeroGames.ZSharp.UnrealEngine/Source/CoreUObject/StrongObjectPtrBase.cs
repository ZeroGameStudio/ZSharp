// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(27)]
public abstract class StrongObjectPtrBase : UnrealObjectWrapperBase
{
	protected StrongObjectPtrBase(Type objectType) : base(objectType, true, false){}
	protected StrongObjectPtrBase(Type objectType, IntPtr unmanaged) : base(objectType, true, false, unmanaged){}
	protected override string _ZCallClassName => "StrongObject";
}


