using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp;

public interface IGCHandle
{
    public GCHandle GCHandle { get; }
}