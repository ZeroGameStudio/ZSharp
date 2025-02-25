// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface IUnrealEnumModel : IUnrealFieldModel, ITypeModel
{
	bool IsFlags { get; }
	IEnumerable<(string, int64)> Values { get; }
}


