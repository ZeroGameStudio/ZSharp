// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface ITypeModel : ISpecifierProvider
{
	IModelRegistry Registry { get; }
	string FullName { get; }
	TypeModelReference BaseType { get; }
}


