// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public interface IUnrealObjectWrapper
{
	bool IsValid { get; }
	bool IsValidEventIfGarbage { get; }
	bool IsNull { get; }
	UObject? Target { get; set; }
	UObject? TargetEvenIfGarbage { get; }
}

public interface IUnrealObjectWrapper<T> : IUnrealObjectWrapper where T : UObject
{
	new T? Target { get; set; }
	new T? TargetEvenIfGarbage { get; }
}


