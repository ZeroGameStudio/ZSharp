// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(41)]
public abstract class UnrealDelegate
{
	
}

public class UnrealDelegate<T> : UnrealDelegate where T : Delegate
{
	
}


