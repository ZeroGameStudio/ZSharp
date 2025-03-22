// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface IUnrealClassModel : IUnrealStructModel, ITypeModel
{
	IReadOnlyList<IUnrealFunctionModel> Functions { get; }
	bool HasUClassConstructor { get; }
	bool IsInternal { get; }
}


