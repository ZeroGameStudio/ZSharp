// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

public partial class UnrealText
{
	
	public UnrealText(string content) : this() => Data = content;

	public override string ToString() => Data;

	public unsafe string Data
	{
		get => new((char*)this.ZCall("ex://Text.GetData", IntPtr.Zero)[-1].Pointer);
		set
		{
			fixed (char* data = value.ToCharArray())
			{
				this.ZCall("ex://Text.SetData", (IntPtr)data);
			}
		}
	}
	
}


