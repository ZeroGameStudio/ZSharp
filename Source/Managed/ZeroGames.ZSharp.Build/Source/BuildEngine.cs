// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Build;

public class BuildEngine : IBuildEngine
{

    public const string KTargetArgumentName = "target";

    public BuildEngine(string[] args)
    {
        _argumentMap = args
            .Distinct()
            .Select(arg => arg.Split('='))
            .Where(values => values.Length > 0)
            .ToDictionary(values => values[0].ToLower(), values => values.Length > 1 ? string.Join('=', values[1..]) : "1");

        _targetMap = AssemblyLoadContext.GetLoadContext(Assembly.GetExecutingAssembly())!.Assemblies
            .SelectMany(asm => asm.GetTypes().Where(type => type.IsAssignableTo(typeof(IBuildTarget)) && type.GetCustomAttribute<BuildTargetAttribute>() is not null))
            .ToDictionary(type => type.GetCustomAttribute<BuildTargetAttribute>()!.Name.ToLower());
    }

    public Task<string> RunAsync()
    {
        IBuildTarget target = GetTarget();
        return target.BuildAsync();
    }

    public string? GetArgument(string key)
    {
        _argumentMap.TryGetValue(key, out var arg);
        return arg;
    }

    private IBuildTarget GetTarget()
    {
        string? target = GetArgument(KTargetArgumentName);
        if (target is null)
        {
            throw new ArgumentException("BuildEngine runs with no target.");
        }

        if (!_targetMap.TryGetValue(target.ToLower(), out var targetType))
        {
            throw new ArgumentException($"Build with invalid target: {target}");
        }

        return (IBuildTarget?)Activator.CreateInstance(targetType, this) ?? throw new InvalidOperationException($"Instantiate target {targetType.FullName} failed.");
    }

    private Dictionary<string, string> _argumentMap;
    private Dictionary<string, Type> _targetMap;

}


