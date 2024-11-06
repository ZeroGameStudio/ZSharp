// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
public sealed partial class UnrealName
{
	
	public UnrealName(string content) : this() => Data = content;

	public override string ToString() => Data;
	
	public static implicit operator UnrealName(string value) => new(value);
	public static implicit operator string(UnrealName value) => value.Data;

	public UnrealString ToUnrealString() => this.ZCall(MasterAlcCache.Instance, "ex://Name.ToString", [ null ])[-1].ReadConjugateChecked<UnrealString>();

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


