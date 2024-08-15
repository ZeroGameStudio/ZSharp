// Copyright Zero Games. All Rights Reserved.

using System.IO;
using System.Reflection;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Core;

internal sealed class SlimAssemblyLoadContext : ZSharpAssemblyLoadContextBase, ISlimAssemblyLoadContext
{
    
    public static SlimAssemblyLoadContext Create(string name)
    {
        if (name == MasterAssemblyLoadContext.KName || name == AssemblyLoadContext.Default.Name || _sInstanceMap.ContainsKey(name))
        {
            throw new Exception();
        }

        return new(name);
    }

    public unsafe ELoadAssemblyErrorCode LoadAssembly(Stream stream, void* args)
    {
        Assembly asm = LoadFromStream(stream);
        if (DllMainStatics.TryInvokeDllMain(asm, args, out var res) && res is not null)
        {
            return ELoadAssemblyErrorCode.Succeed;
        }

        return ELoadAssemblyErrorCode.Succeed;
    }

    public unsafe ECallMethodErrorCode CallMethod(string assemblyName, string typeName, string methodName, void* args)
    {
        throw new NotImplementedException();
    }

    protected override void HandleUnload()
    {
        base.HandleUnload();
        
        if (!_sInstanceMap.TryGetValue(Name!, out var alc) || alc != this)
        {
            throw new Exception();
        }

        _sInstanceMap.Remove(Name!);
    }

    private static readonly Dictionary<string, SlimAssemblyLoadContext> _sInstanceMap = new();

    private SlimAssemblyLoadContext(string name) : base(name)
    {
        _sInstanceMap[name] = this;
    }
    
}


