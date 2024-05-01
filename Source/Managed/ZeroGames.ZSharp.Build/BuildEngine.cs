// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build;

public class BuildEngine : IBuildEngine
{

    public const string KTargetArgumentName = "target";

    public BuildEngine(string[] args)
    {
        var t = args
            .Distinct()
            .Select(arg => arg.Split('='));
        
        _argumentMap = args
            .Distinct()
            .Select(arg => arg.Split('='))
            .Where(values => values.Length > 0)
            .ToDictionary(values => values[0].ToLower(), values => values.Length > 1 ? string.Join('=', values[1..]) : "1");
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
            throw new InvalidOperationException("BuildEngine runs with no target.");
        }

        return new BuildTarget_GenerateSolution(this);
    }

    private Dictionary<string, string> _argumentMap;

}


