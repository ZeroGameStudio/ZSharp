// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public enum EMemberVisibility
{
	Public,
	Protected,
	Private,
}

public interface IMemberModel : ISpecifierProvider
{
	EMemberVisibility Visibility { get; }
}


