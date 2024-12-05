// Copyright Zero Games. All Rights Reserved.

/*
 * This tells Z# the entry point of this assembly is in Game.DllEntry class.
 */
[assembly: DllEntry(typeof(Game.DllEntry))]

namespace Game;

public class DllEntry
{
    /*
     * This tells Z# it is the entry point of this assembly.
     */
    [DllMain]
    public static void DllMain(string[] args)
    {
        UE_LOG(LogTemp, $"============= Game Assembly Loaded! {string.Join("", args)} ===========");
    }
}


