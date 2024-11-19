// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
public sealed partial class UnrealString
{

    public UnrealString(string content) : this() => Data = content;

    public override string ToString() => Data;

    public static implicit operator UnrealString(string value) => new(value);
    public static implicit operator string(UnrealString value) => value.Data;

    public int32 Len => this.ZCall(MasterAlcCache.Instance, "ex://String.Len", 0)[1].Int32;

    public unsafe string Data
    {
        get => new((char*)this.ZCall(MasterAlcCache.Instance, "ex://String.GetData", IntPtr.Zero)[-1].Pointer);
        set
        {
            fixed (char* data = value)
            {
                this.ZCall(MasterAlcCache.Instance, "ex://String.SetData", (IntPtr)data);
            }
        }
    }

}


