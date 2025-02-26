// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ManifestBuilder
{
	
	private static EEnumUnderlyingType TypeUriToEnumUnderlyingType(ITypeUri type)
		=> type.FullName switch
		{
			"System.Byte" => EEnumUnderlyingType.UInt8,
			"System.UInt16" => EEnumUnderlyingType.UInt16,
			"System.UInt32" => EEnumUnderlyingType.UInt32,
			"System.UInt64" => EEnumUnderlyingType.UInt64,
			"System.SByte" => EEnumUnderlyingType.Int8,
			"System.Int16" => EEnumUnderlyingType.Int16,
			"System.Int32" => EEnumUnderlyingType.Int32,
			"System.Int64" => EEnumUnderlyingType.Int64,
			_ => throw new ArgumentOutOfRangeException(nameof(type)),
		};
	
}


