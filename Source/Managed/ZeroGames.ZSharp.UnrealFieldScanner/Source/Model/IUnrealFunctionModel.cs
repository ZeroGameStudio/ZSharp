// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface IUnrealFunctionModel : IUnrealStructModel, IMemberModel, IUnrealSignatureModel
{
	IUnrealClassModel Outer { get; }
	string? EventOverrideName { get; }
}


