﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
public sealed partial class UnrealString
{

    public UnrealString(string content) : this() => Data = content;

    public override string ToString() => Data;

    public int32 Len => this.ZCall("ex://String.Len", 0)[1].Int32;

    public unsafe string Data
    {
        get => new((char*)this.ZCall("ex://String.GetData", IntPtr.Zero)[-1].Pointer);
        set
        {
            fixed (char* data = value)
            {
                this.ZCall("ex://String.SetData", (IntPtr)data);
            }
        }
    }

}


