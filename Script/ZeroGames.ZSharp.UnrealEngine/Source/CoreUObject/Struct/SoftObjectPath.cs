// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class SoftObjectPath : IUnrealObjectPath
{
	public string Path
	{
		get
		{
			KismetSystemLibrary.BreakSoftObjectPath(this, out var path);
			return path;
		}
	}
}


