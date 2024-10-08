// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface IUnrealStructModel : IUnrealFieldModel
{
	IReadOnlyList<IUnrealPropertyModel> Properties { get; }
}


