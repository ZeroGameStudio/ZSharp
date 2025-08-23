// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal static class MetadataScopeExtensions
{
	extension(IMetadataScope @this)
	{
		public string GetAssemblyName() => @this.MetadataScopeType switch
		{
			MetadataScopeType.AssemblyNameReference => @this.Name,
			_ => @this.Name.Remove(@this.Name.Length - 4, 4)
		};
	}
}


