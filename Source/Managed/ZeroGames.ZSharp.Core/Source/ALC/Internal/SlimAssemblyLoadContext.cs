// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

internal sealed class SlimAssemblyLoadContext : ZSharpAssemblyLoadContextBase, ISlimAssemblyLoadContext
{
    
    public static SlimAssemblyLoadContext? Create(string name)
    {
        if (name == MasterAssemblyLoadContext.INSTANCE_NAME || name == Default.Name || _instanceMap.ContainsKey(name))
        {
            return null;
        }

        return new(name);
    }

    protected override void HandleUnload()
    {
        base.HandleUnload();
        
        if (!_instanceMap.TryGetValue(Name!, out var alc) || alc != this)
        {
            throw new Exception();
        }

        _instanceMap.Remove(Name!);
    }

    private static readonly Dictionary<string, SlimAssemblyLoadContext> _instanceMap = new();

    private SlimAssemblyLoadContext(string name) : base(name)
    {
        _instanceMap[name] = this;
    }
    
}


