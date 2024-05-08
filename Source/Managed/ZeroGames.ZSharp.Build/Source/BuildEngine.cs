// Copyright Zero Games. All Rights Reserved.

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

        _targetFactory = new(this);
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

        return _targetFactory.CreateTarget(target);
    }

    private Dictionary<string, string> _argumentMap;
    private BuildTargetFactory _targetFactory;

}


