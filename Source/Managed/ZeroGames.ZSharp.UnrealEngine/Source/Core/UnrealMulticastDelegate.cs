// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(42)]
public abstract class UnrealMulticastDelegate
{
	
}

public class UnrealMulticastDelegate<T> : UnrealMulticastDelegate where T : Delegate
{
	
}


