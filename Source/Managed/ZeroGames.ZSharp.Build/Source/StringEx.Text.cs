// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.Build;

public static class StringEx_Text
{

	public static string Indent(this string @this, int32 count = 1, string indentUnit = "\t")
	{
		StringBuilder unitSb = new();
		for (int32 i = 0; i < count; ++i)
		{
			unitSb.Append(indentUnit);
		}

		string indent = unitSb.ToString();
		string newlineindent = '\n' + indent;

		return @this.Insert(0, indent).Replace("\n", newlineindent);
	}
	
}


