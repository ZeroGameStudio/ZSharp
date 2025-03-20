// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Engine;

internal class StreamableManager : IStreamableManager
{

	public static StreamableManager Get(IntPtr unmanaged)
	{
		if (unmanaged == GlobalInstance._unmanaged)
		{
			return GlobalInstance;
		}

		throw new ArgumentOutOfRangeException(nameof(unmanaged));
	}
	
	public StreamingTask<T> LoadAsync<T>(string path, Lifecycle lifecycle = default) where T : UnrealObject
	{
		using InteropStringArray buffer = new();
		buffer.Add(path);
		IntPtr unmanagedTask = RequestAsyncLoad(buffer.Address, false);
		if (unmanagedTask == IntPtr.Zero)
		{
			return StreamingTask<T>.EmptyTask;
		}
		
		StreamingTask<T> task = new(unmanagedTask, lifecycle);
		_taskMap[unmanagedTask] = task;
		return task;
	}

	public BatchStreamingTask<T> LoadAsync<T>(IEnumerable<string> paths, Lifecycle lifecycle = default, IProgress<BatchStreamingProgress>? progress = null) where T : UnrealObject
	{
		using InteropStringArray buffer = new(paths);
		IntPtr unmanagedTask = RequestAsyncLoad(buffer.Address, progress is not null);
		if (unmanagedTask == IntPtr.Zero)
		{
			return BatchStreamingTask<T>.EmptyTask;
		}
		
		BatchStreamingTask<T> task = new(unmanagedTask, buffer.Count, lifecycle, progress);
		_taskMap[unmanagedTask] = task;
		return task;
	}

	public void Update(IntPtr unmanagedTask, int32 loadedCount)
	{
		if (!_taskMap.TryGetValue(unmanagedTask, out var task))
		{
			UE_WARNING(LogZSharpScriptEngine, $"Streaming task {unmanagedTask} not found.");
			return;
		}
		
		task.Update(loadedCount);
	}

	public void SignalCompletion(IntPtr unmanagedTask)
	{
		if (!_taskMap.Remove(unmanagedTask, out var task))
		{
			UE_WARNING(LogZSharpScriptEngine, $"Streaming task {unmanagedTask} not found.");
			return;
		}
		
		task.SignalCompletion();
	}

	public static unsafe StreamableManager GlobalInstance { get; } = new(StreamableManager_Interop.GetGlobalStreamableManager());

	private unsafe IntPtr RequestAsyncLoad(IntPtr pathsBuffer, bool pushLoadedCount)
		=> StreamableManager_Interop.RequestAsyncLoading(_unmanaged, pathsBuffer, Convert.ToByte(pushLoadedCount));
	
	private StreamableManager(IntPtr unmanaged) => _unmanaged = unmanaged;
	
	private readonly IntPtr _unmanaged;
	
	private readonly Dictionary<IntPtr, IStreamingTask> _taskMap = new();
	
}


