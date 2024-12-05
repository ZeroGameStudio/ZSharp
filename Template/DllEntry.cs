// Copyright Zero Games. All Rights Reserved.

[assembly: DllEntry(typeof(Game.DllEntry))]

namespace Game;

public class DllEntry
{
    [DllMain]
    public static void DllMain()
    {
        UE_LOG(LogTemp, "============= Game Assembly Loaded! ===========");
    }
}


