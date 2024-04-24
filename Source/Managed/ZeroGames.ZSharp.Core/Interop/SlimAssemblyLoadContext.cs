// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Core;

public class SlimAssemblyLoadContext : ZSharpAssemblyLoadContextBase
{
    
    internal static SlimAssemblyLoadContext Create(string name)
    {
        if (name == MasterAssemblyLoadContext.KName || name == AssemblyLoadContext.Default.Name || _sInstanceMap.ContainsKey(name))
        {
            throw new Exception();
        }

        return new(name);
    }

    protected override void HandleUnload()
    {
        base.HandleUnload();
        
        if (!_sInstanceMap.TryGetValue(Name!, out var alc) || alc != this)
        {
            throw new Exception();
        }

        _sInstanceMap.Remove(Name!);
            
        Logger.Log($"Slim ALC Unloaded, name: {Name}, handle: {GCHandle.ToIntPtr(GCHandle)}");
    }

    private static readonly Dictionary<string, SlimAssemblyLoadContext> _sInstanceMap = new();

    private SlimAssemblyLoadContext(string name) : base(name)
    {
        _sInstanceMap[name] = this;
    }
    
}