// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(24)]
public class WeakObjectPtr : PlainExportedObjectBase
{
	
}

public class WeakObjectPtr<T> : WeakObjectPtr where T : UnrealObject?
{
	
}


