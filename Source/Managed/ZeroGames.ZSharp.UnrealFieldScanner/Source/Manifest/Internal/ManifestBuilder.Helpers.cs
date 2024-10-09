// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ManifestBuilder
{

	private string GetUnrealFieldPath(ITypeModel typeModel) => typeModel.GetSpecifier<UnrealFieldPathAttribute>()!.Path;
	
}


