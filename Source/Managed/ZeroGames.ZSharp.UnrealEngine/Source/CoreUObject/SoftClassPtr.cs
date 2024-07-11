// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(22)]
public class SoftClassPtr : PlainExportedObjectBase
{
	
}

public class SoftClassPtr<T> : SoftClassPtr where T : UnrealObject
{
	
}


