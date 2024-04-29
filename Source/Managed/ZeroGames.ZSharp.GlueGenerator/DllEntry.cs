// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Loader;
using ZeroGames.ZSharp.Core;

[assembly: DllEntry(typeof(ZeroGames.ZSharp.GlueGenerator.DllEntry))]

namespace ZeroGames.ZSharp.GlueGenerator;

internal static class DllEntry
{
    
    [StructLayout(LayoutKind.Sequential)]
    private unsafe struct DllMainArgs
    {
        public delegate* unmanaged<void> OnComplete;
        public delegate* unmanaged<int32, int32, void> OnUpdate;
    }

    [DllMain]
    private static unsafe int32 DllMain(DllMainArgs* rawArgs)
    {
        Logger.Log("===================== Glue Generator Begin =====================");

        ProcessAsync((processed, total) => { rawArgs->OnUpdate(processed, total); }, () => { rawArgs->OnComplete(); });
        
        return 0;
    }

    private static async void ProcessAsync(Action<int32, int32> onUpdate, Action onComplete)
    {
        int32 processed = 0;
        int32 numFiles = 10000;
        int32 blockSize = 64;
        List<int32> files = new(numFiles);
        for (int32 i = 0; i < numFiles; ++i)
        {
            files.Add(new Random().Next());
        }

        int32 numBlocks = numFiles / blockSize + 1;
        List<(int32 Start, int32 End)> blocks = new(numBlocks);
        for (int32 i = 0; i < numBlocks; ++i)
        {
            blocks.Add((i * blockSize, Math.Min((i + 1) * blockSize - 1, numFiles)));
        }
        
        onUpdate(processed, numFiles);
        
        await Parallel.ForEachAsync(blocks, (startEndPos, cancellationToken) =>
        {
            return new(Task.Run(() =>
            {
                for (int32 i = startEndPos.Start; i < startEndPos.End; ++i)
                {
                    Interlocked.Increment(ref processed);
                    Thread.Sleep(1);
                    onUpdate(processed, numFiles);
                }
            }, cancellationToken));
        });

        Logger.Log("===================== Glue Generator End =====================");
            
        onComplete();
            
        AssemblyLoadContext.GetLoadContext(Assembly.GetExecutingAssembly())!.Unload();
    }
    
}


