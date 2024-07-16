// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(21)]
public abstract class SubclassOfBase : UnrealObjectWrapperBase
{
	protected SubclassOfBase(Type objectType) : base(objectType, true, true){}
	protected SubclassOfBase(Type objectType, IntPtr unmanaged) : base(objectType, true, true, unmanaged){}
}


