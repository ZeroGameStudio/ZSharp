// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(27)]
public class StrongObjectPtr : PlainExportedObjectBase
{
	
}

public class StrongObjectPtr<T> : StrongObjectPtr where T : UnrealObject?
{
	
}


