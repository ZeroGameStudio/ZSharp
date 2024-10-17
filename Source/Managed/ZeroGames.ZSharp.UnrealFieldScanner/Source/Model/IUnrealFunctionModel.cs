// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface IUnrealFunctionModel : IUnrealStructModel, IMemberModel
{
	IUnrealClassModel Outer { get; }
	string? EventOverrideName { get; }
}

public static class UnrealFunctionModelExtensions
{
	
	public static bool HasReturnValue(this IUnrealFunctionModel @this) => @this.Properties.Any(p => p.Role == EPropertyRole.Return);

}


