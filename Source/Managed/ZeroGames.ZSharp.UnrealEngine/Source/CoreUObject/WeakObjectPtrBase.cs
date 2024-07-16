// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(24)]
public abstract class WeakObjectPtrBase : UnrealObjectWrapperBase
{
	protected WeakObjectPtrBase(Type objectType) : base(objectType, true, false){}
	protected WeakObjectPtrBase(Type objectType, IntPtr unmanaged) : base(objectType, true, false, unmanaged){}
}


