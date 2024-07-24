// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public interface IExplicitLifecycle
{
	bool IsExpired { get; }
	/// <summary>
	/// Event on object expired. You can only use the parameter as a handle and should never dereference it.
	/// </summary>
	event Action<IExplicitLifecycle> OnExpired;
}


