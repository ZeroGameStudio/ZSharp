// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public interface IUnrealObjectWrapper
{
	bool IsValid { get; }
	bool IsValidEventIfGarbage { get; }
	bool IsNull { get; }
	UnrealObject? Target { get; set; }
	UnrealObject? TargetEvenIfGarbage { get; }
}

public interface IUnrealObjectWrapper<T> : IUnrealObjectWrapper where T : UnrealObject
{
	new T? Target { get; set; }
	new T? TargetEvenIfGarbage { get; }
}


