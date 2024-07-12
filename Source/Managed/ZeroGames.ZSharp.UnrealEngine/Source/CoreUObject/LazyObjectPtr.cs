// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(25)]
public class LazyObjectPtr : PlainExportedObjectBase
{
	
}

public class LazyObjectPtr<T> : LazyObjectPtr where T : UnrealObject?
{
	
}


