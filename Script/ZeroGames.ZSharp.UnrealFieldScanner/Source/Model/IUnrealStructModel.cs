// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface IUnrealStructModel : IUnrealFieldModel
{
	IReadOnlyList<IUnrealPropertyModel> Properties { get; }
}

public static class UnrealStructModelExtensions
{
	extension(IUnrealStructModel @this)
	{
		public bool HasReturnValue
			=> @this.Properties.Any(p => p.Role == EPropertyRole.Return);
	}
}


