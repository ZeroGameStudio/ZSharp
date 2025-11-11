// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public interface IStreamableManager
{
	public static IStreamableManager GlobalInstance => StreamableManager.GlobalInstance;
	
	StreamingTask<T> LoadAsync<T>(string path, Lifetime lifetime = default) where T : UObject;
	BatchStreamingTask<T> LoadAsync<T>(IEnumerable<string> paths, Lifetime lifetime = default, IProgress<BatchStreamingProgress>? progress = null) where T : UObject;
}

public static class StreamableManagerExtensions
{
	extension(IStreamableManager @this)
	{
		public StreamingTask<T> LoadAsync<T>(IUnrealObjectPath path, Lifetime lifetime = default) where T : UObject
			=> @this.LoadAsync<T>(path.Path, lifetime);
	
		public StreamingTask<UClass> LoadAsync(IUnrealClassPath path, Lifetime lifetime = default)
			=> @this.LoadAsync<UClass>(path.Path, lifetime);

		public BatchStreamingTask<T> LoadAsync<T>(IEnumerable<IUnrealObjectPath> paths, Lifetime lifetime = default, IProgress<BatchStreamingProgress>? progress = null) where T : UObject
			=> @this.LoadAsync<T>(paths.Select(p => p.Path), lifetime, progress);
	
		public  BatchStreamingTask<UClass> LoadAsync(IEnumerable<IUnrealClassPath> paths, Lifetime lifetime = default, IProgress<BatchStreamingProgress>? progress = null)
			=> @this.LoadAsync<UClass>(paths.Select(p => p.Path), lifetime, progress);
	}
}


