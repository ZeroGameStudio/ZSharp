// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ManifestBuilder
{

	private UnrealEnumDefinition ScanEnumModel(IUnrealEnumModel enumModel)
	{
		UnrealEnumDefinition result = new()
		{
			Name = enumModel.Name,
			EnumFlags = enumModel.IsFlags ? EEnumFlags.Flags : EEnumFlags.None,
		};

		foreach (var (name, value) in enumModel.Values)
		{
			result.ValueMap[name] = value;
		}
		
		return result;
	}
	
}


