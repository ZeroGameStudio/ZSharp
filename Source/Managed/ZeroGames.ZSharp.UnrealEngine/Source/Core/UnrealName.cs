// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

public partial class UnrealName
{
	
	public UnrealName(string content) : this() => Data = content;

	public override string ToString() => Data;

	public UnrealString ToUnrealString() => this.ZCall("ex://Name.ToString", [ null ])[-1].ReadConjugate<UnrealString>()!;

	public unsafe string Data
	{
		get => ToUnrealString().Data;
		set
		{
			fixed (char* data = value.ToCharArray())
			{
				this.ZCall("ex://Name.SetData", (IntPtr)data);
			}
		}
	}
	
}


