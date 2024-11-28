// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public interface IPersistentObjectWrapper<T> : IUnrealObjectWrapper<T> where T : UnrealObject
{
	bool IsPending { get; }
}


