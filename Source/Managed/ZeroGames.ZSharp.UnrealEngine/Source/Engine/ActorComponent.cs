// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public partial class ActorComponent
{

	public Actor? Owner => CallUnrealFunction<Actor>("GetOwner");

}


