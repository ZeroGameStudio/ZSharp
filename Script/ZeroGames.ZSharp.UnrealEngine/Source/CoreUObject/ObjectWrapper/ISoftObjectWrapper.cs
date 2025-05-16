// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public interface ISoftObjectWrapper<T> : IPersistentObjectWrapper<T> where T : UObject
{
	bool TryLoad([NotNullWhen(true)] out T? target);
}


