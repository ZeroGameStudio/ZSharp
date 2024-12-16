// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class AssetManager
{

	public static AssetManager Instance { get; } = Engine.Instance.AssetManager!;

	public IStreamableManager StreamableManager => IStreamableManager.GlobalInstance;

}


