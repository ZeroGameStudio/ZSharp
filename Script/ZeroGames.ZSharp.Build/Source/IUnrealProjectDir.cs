﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build;

public interface IUnrealProjectDir
{
	string UnrealProjectDir { get; }
	bool IsValid => Directory.GetFiles(UnrealProjectDir, "*.uproject").Length > 0;
}


