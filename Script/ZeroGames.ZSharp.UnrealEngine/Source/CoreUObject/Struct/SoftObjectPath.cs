// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class FSoftObjectPath : IUnrealObjectPath
{
	public string Path
	{
		get
		{
			UKismetSystemLibrary.BreakSoftObjectPath(this, out var path);
			return path;
		}
	}
}


