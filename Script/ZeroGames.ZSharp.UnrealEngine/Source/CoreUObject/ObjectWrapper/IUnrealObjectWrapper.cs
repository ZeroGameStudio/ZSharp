// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public interface IUnrealObjectWrapper
{
	bool IsValid { get; }
	bool IsValidEventIfGarbage { get; }
	bool IsNull { get; }
}

public interface IUnrealObjectWrapper<T> : IUnrealObjectWrapper where T : UnrealObject
{
	T? Target { get; set; }
	T? TargetEvenIfGarbage { get; }
}


