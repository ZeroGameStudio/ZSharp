// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

public interface IStreamableManager
{
	public static IStreamableManager GlobalInstance => StreamableManager.GlobalInstance;
	
	StreamingTask<T> LoadAsync<T>(string path, Lifecycle lifecycle = default) where T : UnrealObject;
	BatchStreamingTask<T> LoadAsync<T>(IEnumerable<string> paths, Lifecycle lifecycle = default, IProgress<BatchStreamingProgress>? progress = null) where T : UnrealObject;
}

public static class StreamableManagerExtensions
{

	public static StreamingTask<T> LoadAsync<T>(this IStreamableManager @this, IUnrealObjectPath path, Lifecycle lifecycle = default) where T : UnrealObject
		=> @this.LoadAsync<T>(path.Path, lifecycle);
	
	public static StreamingTask<UnrealClass> LoadAsync(this IStreamableManager @this, IUnrealClassPath path, Lifecycle lifecycle = default)
		=> @this.LoadAsync<UnrealClass>(path.Path, lifecycle);

	public static BatchStreamingTask<T> LoadAsync<T>(this IStreamableManager @this, IEnumerable<IUnrealObjectPath> paths, Lifecycle lifecycle = default, IProgress<BatchStreamingProgress>? progress = null) where T : UnrealObject
		=> @this.LoadAsync<T>(paths.Select(p => p.Path), lifecycle, progress);
	
	public static BatchStreamingTask<UnrealClass> LoadAsync(this IStreamableManager @this, IEnumerable<IUnrealClassPath> paths, Lifecycle lifecycle = default, IProgress<BatchStreamingProgress>? progress = null)
		=> @this.LoadAsync<UnrealClass>(paths.Select(p => p.Path), lifecycle, progress);
	
}


