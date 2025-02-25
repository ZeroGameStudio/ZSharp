// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface IUnrealEnumModel : IUnrealFieldModel, ITypeModel
{
	bool IsFlags { get; }
	IReadOnlyList<IUnrealEnumFieldModel> Fields { get; }
}


