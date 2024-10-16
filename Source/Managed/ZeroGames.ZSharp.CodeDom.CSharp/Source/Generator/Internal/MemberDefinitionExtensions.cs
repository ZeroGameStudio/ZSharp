// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

internal static class MemberDefinitionExtensions
{
	
	public static string GetVisibilityText(this MemberDefinitionBase @this) => @this.Visibility switch
	{
		EMemberVisibility.Default => string.Empty,
		EMemberVisibility.Public => "public",
		EMemberVisibility.Internal => "internal",
		EMemberVisibility.Protected => "protected",
		EMemberVisibility.Private => "private",
		_ => throw new NotSupportedException()
	};

	public static string GetModifiersText(this MemberDefinitionBase @this)
	{
		List<string> modifiers = new();
		
		if ((@this.Modifiers & EMemberModifiers.Abstract) != EMemberModifiers.None)
		{
			modifiers.Add("abstract");
		}
		
		if ((@this.Modifiers & EMemberModifiers.Sealed) != EMemberModifiers.None)
		{
			modifiers.Add("sealed");
		}
		
		if ((@this.Modifiers & EMemberModifiers.Static) != EMemberModifiers.None)
		{
			modifiers.Add("static");
		}
		
		if ((@this.Modifiers & EMemberModifiers.Unsafe) != EMemberModifiers.None)
		{
			modifiers.Add("unsafe");
		}
		
		if ((@this.Modifiers & EMemberModifiers.Partial) != EMemberModifiers.None)
		{
			modifiers.Add("partial");
		}

		return string.Join(" ", modifiers);
	}
	
}


