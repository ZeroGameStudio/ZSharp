// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface IUnrealDelegateModel : IUnrealStructModel, ITypeModel, IUnrealSignatureModel
{
	IUnrealClassModel? Outer { get; }
}


