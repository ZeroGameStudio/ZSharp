// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface IUnrealScriptStructModel : IUnrealStructModel, ITypeModel
{
	bool HasNetSerialize { get; }
}


