using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

public interface IGCHandle
{
    public GCHandle GCHandle { get; }
}