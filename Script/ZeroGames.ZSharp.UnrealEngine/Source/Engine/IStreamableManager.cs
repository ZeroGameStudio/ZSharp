// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public interface IStreamableManager
{
	public static IStreamableManager GlobalInstance => StreamableManager.GlobalInstance;
	
	StreamingTask<T> LoadAsync<T>(string path, Lifecycle lifecycle = default) where T : UObject;
	BatchStreamingTask<T> LoadAsync<T>(IEnumerable<string> paths, Lifecycle lifecycle = default, IProgress<BatchStreamingProgress>? progress = null) where T : UObject;
}

public static class StreamableManagerExtensions
{

	public static StreamingTask<T> LoadAsync<T>(this IStreamableManager @this, IUnrealObjectPath path, Lifecycle lifecycle = default) where T : UObject
		=> @this.LoadAsync<T>(path.Path, lifecycle);
	
	public static StreamingTask<UClass> LoadAsync(this IStreamableManager @this, IUnrealClassPath path, Lifecycle lifecycle = default)
		=> @this.LoadAsync<UClass>(path.Path, lifecycle);

	public static BatchStreamingTask<T> LoadAsync<T>(this IStreamableManager @this, IEnumerable<IUnrealObjectPath> paths, Lifecycle lifecycle = default, IProgress<BatchStreamingProgress>? progress = null) where T : UObject
		=> @this.LoadAsync<T>(paths.Select(p => p.Path), lifecycle, progress);
	
	public static BatchStreamingTask<UClass> LoadAsync(this IStreamableManager @this, IEnumerable<IUnrealClassPath> paths, Lifecycle lifecycle = default, IProgress<BatchStreamingProgress>? progress = null)
		=> @this.LoadAsync<UClass>(paths.Select(p => p.Path), lifecycle, progress);
	
}


