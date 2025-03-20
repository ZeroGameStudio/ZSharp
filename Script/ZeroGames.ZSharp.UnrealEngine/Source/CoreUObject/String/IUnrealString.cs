// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public interface IUnrealString
{
	[AllowNull]
	string Data { get; set; }
}


