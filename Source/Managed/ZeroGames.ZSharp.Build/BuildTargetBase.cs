// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build;

public abstract class BuildTargetBase : IBuildTarget
{

    protected BuildTargetBase(IBuildEngine engine)
    {
        Engine = engine;
    }
    
    public virtual Task<string> BuildAsync()
    {
        return Task.Run(() => "");
    }
    
    public IBuildEngine Engine { get; }
    
}


