// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public enum EPropertyRole
{
	Member,
	Parameter,
	Return,
}

public interface IUnrealPropertyModel : IUnrealFieldModel, IMemberModel
{
	EPropertyRole Role { get; }
	TypeModelReference Type { get; }
	IPropertyAccessorModel? Getter { get; }
	IPropertyAccessorModel? Setter { get; }
}


