// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface IUnrealClassModel : IUnrealStructModel, ITypeModel
{
	IReadOnlyList<IUnrealFunctionModel> Functions { get; }
	public bool IsInternal { get; }
}


