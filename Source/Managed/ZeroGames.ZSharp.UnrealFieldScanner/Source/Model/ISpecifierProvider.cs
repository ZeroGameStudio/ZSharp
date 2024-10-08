// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface ISpecifierProvider
{
	bool HasSpecifier(Type attributeType);
	IUnrealReflectionSpecifier? GetSpecifier(Type attributeType);
	IReadOnlyCollection<IUnrealReflectionSpecifier> Specifiers { get; }
}


