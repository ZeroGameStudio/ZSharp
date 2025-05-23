﻿// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

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
    
    public GCHandle GCHandle { get; }

    protected override void HandleUnload()
    {
        if (_instanceMap.TryGetValue(Name!, out var alc) && alc == this)
        {
            _instanceMap.Remove(Name!);
        }
        else
        {
            CoreLog.Warning($"Slim ALC {Name} doesn't exist!");
        }
        
        GCHandle.Free();

        base.HandleUnload();
    }
    
    private SlimAssemblyLoadContext(string name) : base(name)
    {
        GCHandle = GCHandle.Alloc(this, GCHandleType.Weak);
        _instanceMap[name] = this;
    }

    private static readonly Dictionary<string, SlimAssemblyLoadContext> _instanceMap = new();
    
}


