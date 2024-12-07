// Copyright Zero Games. All Rights Reserved.

/*
 * This tells Z# the entry point of this assembly is in Game.DllEntry class.
 */
[assembly: DllEntry(typeof(Game.DllEntry))]

namespace Game;

internal class DllEntry
{
    /*
     * This tells Z# it is the entry point of this assembly.
     */
    [DllMain]
    private static void DllMain(string[] args)
    {
        UE_LOG(LogTemp, $"============= Game Assembly Loaded! {string.Join("", args)} ===========");
    }
}


