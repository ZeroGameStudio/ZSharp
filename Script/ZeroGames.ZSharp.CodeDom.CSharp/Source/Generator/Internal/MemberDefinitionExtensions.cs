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
		
		if (@this.Modifiers.HasFlag(EMemberModifiers.Abstract))
		{
			modifiers.Add("abstract");
		}
		
		if (@this.Modifiers.HasFlag(EMemberModifiers.Virtual))
		{
			modifiers.Add("virtual");
		}
		
		if (@this.Modifiers.HasFlag(EMemberModifiers.Sealed))
		{
			modifiers.Add("sealed");
		}
		
		if (@this.Modifiers.HasFlag(EMemberModifiers.Static))
		{
			modifiers.Add("static");
		}
		
		if (@this.Modifiers.HasFlag(EMemberModifiers.Unsafe))
		{
			modifiers.Add("unsafe");
		}
		
		if (@this.Modifiers.HasFlag(EMemberModifiers.Partial))
		{
			modifiers.Add("partial");
		}

		return string.Join(" ", modifiers);
	}
	
}


