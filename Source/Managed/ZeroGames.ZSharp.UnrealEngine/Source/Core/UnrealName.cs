// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

public partial class UnrealName
{
	
	public UnrealName(string content) : this() => Data = content;

	public override string ToString() => Data;

	public unsafe string Data
	{
		get => new((char*)this.ZCall("ex://Name.GetData", IntPtr.Zero)[-1].Pointer);
		set
		{
			fixed (char* data = value.ToCharArray())
			{
				this.ZCall("ex://Name.SetData", (IntPtr)data);
			}
		}
	}
	
}


