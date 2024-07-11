// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(23)]
public class SoftObjectPtr : PlainExportedObjectBase
{
	
}

public class SoftObjectPtr<T> : SoftObjectPtr where T : UnrealObject
{
	
}


