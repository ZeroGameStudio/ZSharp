// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal static class MetadataScopeExtensions
{

	public static string GetAssemblyName(this IMetadataScope scope) => scope.MetadataScopeType switch
	{
		MetadataScopeType.AssemblyNameReference => scope.Name,
		_ => scope.Name.Remove(scope.Name.Length - 4, 4)
	};

}


