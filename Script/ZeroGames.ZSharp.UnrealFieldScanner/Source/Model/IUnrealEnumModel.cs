// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public interface IUnrealEnumModel : IUnrealFieldModel, ITypeModel
{
	bool IsFlags { get; } // This is set by non-specifier attribute [System.Flags] so we can't use specifier processor.
	IReadOnlyList<IUnrealEnumFieldModel> Fields { get; }
}


