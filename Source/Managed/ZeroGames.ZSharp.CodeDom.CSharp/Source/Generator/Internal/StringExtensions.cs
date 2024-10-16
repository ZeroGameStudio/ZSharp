// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.CodeDom.CSharp;

internal static class StringExtensions
{
	
	public static string Indent(this string @this, int32 count = 0, string indentUnit = "\t")
	{
		if (count <= 0)
		{
			return @this;
		}
		
		StringBuilder unitSb = new();
		for (int32 i = 0; i < count; ++i)
		{
			unitSb.Append(indentUnit);
		}

		string indent = unitSb.ToString();
		string newlineindent = Environment.NewLine + indent;

		return @this.Insert(0, indent).Replace(Environment.NewLine, newlineindent);
	}
	
}


