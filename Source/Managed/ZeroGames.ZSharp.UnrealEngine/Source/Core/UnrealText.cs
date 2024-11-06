// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
public sealed partial class UnrealText
{
	
	public UnrealText(string content) : this() => Data = content;

	public override string ToString() => Data;
	public static implicit operator UnrealText(string value) => new(value);
	public static implicit operator string(UnrealText value) => value.Data;

	public unsafe string Data
	{
		get => new((char*)this.ZCall(MasterAlcCache.Instance, "ex://Text.GetData", IntPtr.Zero)[-1].Pointer);
		set
		{
			fixed (char* data = value)
			{
				this.ZCall(MasterAlcCache.Instance, "ex://Text.SetData", (IntPtr)data);
			}
		}
	}
	
}


