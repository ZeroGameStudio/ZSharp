﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public interface IWeakObjectWrapper<T> : IUnrealObjectWrapper<T> where T : UnrealObject
{
	bool IsStale { get; }
	bool IsStaleExcludingGarbage { get; }
}

