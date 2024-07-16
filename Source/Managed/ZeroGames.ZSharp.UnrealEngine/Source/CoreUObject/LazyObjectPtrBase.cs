// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(25)]
public abstract class LazyObjectPtrBase : UnrealObjectWrapperBase
{
	protected LazyObjectPtrBase(Type objectType) : base(objectType, true, false){}
	protected LazyObjectPtrBase(Type objectType, IntPtr unmanaged) : base(objectType, true, false, unmanaged){}
}


