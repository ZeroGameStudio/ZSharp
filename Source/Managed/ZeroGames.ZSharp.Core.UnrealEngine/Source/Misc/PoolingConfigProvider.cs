// Copyright Zero Games. All Rights Reserved.

using ZeroGames.Pooling;

namespace ZeroGames.ZSharp.Core.UnrealEngine;

public class PoolingConfigProvider<T> : IConfigProvider<T> where T : class, new()
{

	public void GetConfig(out ObjectPoolConfig config) => config = _config;

	private static int32 GetFromIniConfig(string prefix, string name, int32 fallback)
	{
		if (!GConfig.TryGetInt32(GZSharpIni, SECTION, $"{prefix}.{name}", out var result))
		{
			result = GConfig.GetInt32OrDefault(GZSharpIni, SECTION, name, fallback);
		}
		
		return result;
	}

	static PoolingConfigProvider()
	{
		Type type = typeof(T);
		string prefix = type.IsGenericType ? type.GetGenericTypeDefinition().FullName! : type.FullName!;
		
		_config = new()
		{
			PrecacheCount = GetFromIniConfig(prefix, nameof(ObjectPoolConfig.PrecacheCount), FALLBACK_PRECACHE_COUNT),
			MaxAliveCount = GetFromIniConfig(prefix, nameof(ObjectPoolConfig.MaxAliveCount), FALLBACK_MAX_ALIVE_COUNT),
		};
	}

	private const string SECTION = "Managed.Pooling";
	
	private const int32 FALLBACK_PRECACHE_COUNT = 0;
	private const int32 FALLBACK_MAX_ALIVE_COUNT = 1 << 5;
	
	private static readonly ObjectPoolConfig _config;

}


