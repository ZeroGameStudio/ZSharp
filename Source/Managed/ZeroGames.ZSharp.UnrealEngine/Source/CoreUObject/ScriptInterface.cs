// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(26)]
public class ScriptInterface : PlainExportedObjectBase
{
	
}

public class ScriptInterface<T> : ScriptInterface where T : IUnrealInterface
{
	
}


