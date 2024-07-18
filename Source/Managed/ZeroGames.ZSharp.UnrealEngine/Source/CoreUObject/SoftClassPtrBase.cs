// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(22)]
public abstract class SoftClassPtrBase : UnrealObjectWrapperBase
{
	protected SoftClassPtrBase(Type objectType) : base(objectType, true, true){}
	protected SoftClassPtrBase(Type objectType, IntPtr unmanaged) : base(objectType, true, true, unmanaged){}
	protected override string _ZCallClassName => "SoftClass";
}


