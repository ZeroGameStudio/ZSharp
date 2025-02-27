// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface IUnrealStructModel : IUnrealFieldModel
{
	IReadOnlyList<IUnrealPropertyModel> Properties { get; }
}

public static class UnrealStructModelExtensions
{
	
	public static bool HasReturnValue(this IUnrealStructModel @this) => @this.Properties.Any(p => p.Role == EPropertyRole.Return);

}


