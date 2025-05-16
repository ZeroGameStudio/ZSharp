// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class UAssetManager
{

	public static UAssetManager Instance { get; } = UEngine.Instance.AssetManager!;

	public IStreamableManager StreamableManager => IStreamableManager.GlobalInstance;

}


