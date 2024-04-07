using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp;

/// <summary>
/// A conjugate is object used by both managed and unmanaged side.
/// Unmanaged side keeps reference to it by a GCHandle.
/// This means you must free the handle in Release() and manually call it somewhere, otherwise there will be a memory leak.
/// </summary>
public interface IConjugate
{
    public void Release();
    public GCHandle GCHandle { get; }
}


