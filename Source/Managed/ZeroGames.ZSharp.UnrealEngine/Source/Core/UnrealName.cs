// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
public sealed partial class UnrealName
{
	
	public UnrealName(string content) : this() => Data = content;

	public override string ToString() => Data;

	public UnrealString ToUnrealString() => this.ZCall(MasterAlcCache.Instance, "ex://Name.ToString", [ null ])[-1].ReadConjugate<UnrealString>()!;

	public unsafe string Data
	{
		get => ToUnrealString().Data;
		set
		{
			fixed (char* data = value)
			{
				this.ZCall(MasterAlcCache.Instance, "ex://Name.SetData", (IntPtr)data);
			}
		}
	}
	
}


