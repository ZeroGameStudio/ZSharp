// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(23)]
public abstract class SoftObjectPtrBase : UnrealObjectWrapperBase
{
	protected SoftObjectPtrBase(Type objectType) : base(objectType, true, false){}
	protected SoftObjectPtrBase(Type objectType, IntPtr unmanaged) : base(objectType, true, false, unmanaged){}
}


