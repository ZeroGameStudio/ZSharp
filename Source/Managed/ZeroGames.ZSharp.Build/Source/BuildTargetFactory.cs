// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Build;

public class BuildTargetFactory : IBuildTargetFactory
{

	public BuildTargetFactory(IBuildEngine engine)
	{
		_engine = engine;
	}

	public IBuildTarget CreateTarget(string name)
	{
		if (!_targetMap.TryGetValue(name.ToLower(), out var targetType))
		{
			throw new ArgumentException($"Build with invalid target: {name}");
		}

		ConstructorInfo? ctor = targetType.GetConstructors(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance).FirstOrDefault(ctor => ctor.GetCustomAttribute<FactoryConstructorAttribute>() is not null);
		if (ctor is null)
		{
			throw new InvalidOperationException($"Factory constructor not found for target {targetType.FullName}");
		}

		List<object?> parameters = new();
		foreach (var para in ctor.GetParameters())
		{
			ArgumentAttribute? attr = para.GetCustomAttribute<ArgumentAttribute>();
			if (attr is null)
			{
				Type engineType = _engine.GetType();
				if (!para.ParameterType.IsAssignableFrom(engineType))
				{
					throw new ArgumentException($"Non-argument parameter {para.Name} is not fit to engine type {engineType}");
				}
				
				parameters.Add(_engine);
			}
			else
			{
				string? value = _engine.GetArgument(attr.Name);
				if (value is not null)
				{
					parameters.Add(Parse(para.ParameterType, value));
				}
				else if (para.HasDefaultValue)
				{
					parameters.Add(para.DefaultValue);
				}
				else
				{
					throw new ArgumentException($"Missing required argument {attr.Name}");
				}
			}
		}
		
		return (IBuildTarget)ctor.Invoke(parameters.ToArray());
	}

	private object Parse(Type type, string value)
	{
		if (!_parserMap.ContainsKey(type))
		{
			throw new ArgumentException($"Unsupported argument type {type.FullName}.");
		}

		return _parserMap[type](value);
	}

	static BuildTargetFactory()
	{
		_targetMap = AssemblyLoadContext.GetLoadContext(Assembly.GetExecutingAssembly())!.Assemblies
			.SelectMany(asm => asm.GetTypes().Where(type => type.IsAssignableTo(typeof(IBuildTarget)) && type.GetCustomAttribute<BuildTargetAttribute>() is not null))
			.ToDictionary(type => type.GetCustomAttribute<BuildTargetAttribute>()!.Name.ToLower());

		_parserMap = new()
		{
			{ typeof(string), value => value },
			{ typeof(uint8), value => uint8.Parse(value) },
			{ typeof(uint16), value => uint16.Parse(value) },
			{ typeof(uint32), value => uint32.Parse(value) },
			{ typeof(uint64), value => uint64.Parse(value) },
			{ typeof(int8), value => int8.Parse(value) },
			{ typeof(int16), value => int16.Parse(value) },
			{ typeof(int32), value => int32.Parse(value) },
			{ typeof(int64), value => int64.Parse(value) },
			{ typeof(float), value => float.Parse(value) },
			{ typeof(double), value => double.Parse(value) },
			{ typeof(bool), value => bool.Parse(value) },
		};
	}

	private static Dictionary<string, Type> _targetMap;
	private static Dictionary<Type, Func<string, object>> _parserMap;
		
	private IBuildEngine _engine;

}


