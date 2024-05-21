// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

public static class ZSharpActorStatics
{

    [ZCall]
    public static void BeginPlay(UnrealObject actor)
    {
        var x = new UnrealString();
        Logger.Log($"ZSharp BeginPlay Actor Name: {actor.Name}");
    }

    [ZCall]
    public static void Tick(UnrealObject actor, float deltaTime)
    {
        PlainExportedObjectBase? o = new Random().Next(0, 10) switch
        {
            1 => new UnrealString(),
            2 => new UnrealName(),
            3 => new UnrealText(),
            4 => new WeakObjectPtr(),
            5 => new SoftObjectPtr(),
            6 => new LazyObjectPtr(),
            7 => new StrongObjectPtr(),
            8 => new SoftClassPtr(),
            9 => new ScriptInterface(),
            _ => null,
        };
        Logger.Log($"ZSharp Tick Actor Name: {actor.Name} DeltaTime: {deltaTime} ManagedType: {actor.GetType().FullName} UnrealClass: {actor.Class.Name}");
    }

    [ZCall]
    public static void EndPlay(UnrealObject actor)
    {
        Logger.Log($"ZSharp EndPlay Actor Name: {actor.Name}");
    }

}