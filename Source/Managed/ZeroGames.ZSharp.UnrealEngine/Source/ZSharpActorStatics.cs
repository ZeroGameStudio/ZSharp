// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core;
using ZeroGames.ZSharp.UnrealEngine.Core;
using ZeroGames.ZSharp.UnrealEngine.CoreUObject;

namespace ZeroGames.ZSharp.UnrealEngine;

public static class ZSharpActorStatics
{

    [ZCall]
    public static void BeginPlay(UnrealObject actor)
    {
        Logger.Log($"ZSharp BeginPlay Actor Name: {actor.Name}");
    }

    [ZCall]
    public static void Tick(UnrealObject actor, float deltaTime)
    {
        Logger.Log($"ZSharp Tick Actor Name: {actor.Name} DeltaTime: {deltaTime}");
    }

    [ZCall]
    public static void EndPlay(UnrealObject actor)
    {
        Logger.Log($"ZSharp EndPlay Actor Name: {actor.Name}");
    }

}