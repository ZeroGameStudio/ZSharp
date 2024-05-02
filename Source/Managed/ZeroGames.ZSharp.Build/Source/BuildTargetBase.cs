// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build;

public abstract class BuildTargetBase : IBuildTarget
{

    protected BuildTargetBase(IBuildEngine engine)
    {
        Engine = engine;
    }

    public abstract Task<string> BuildAsync();
    
    public IBuildEngine Engine { get; }
    
}


