// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class CharacterMovementComponent
{
	
	public float MaxWalkSpeed
	{
		get => ReadUnrealProperty<float>("MaxWalkSpeed");
		set => WriteUnrealProperty("MaxWalkSpeed", value);
	}
	
}