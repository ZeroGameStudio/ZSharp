// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal static class CustomAttributeProviderExtensions
{
	extension(ICustomAttributeProvider @this)
	{
		public bool HasCustomAttribute(string attributeTypeFullName)
		{
			return @this.CustomAttributes.Any(attr => attr.AttributeType.FullName == attributeTypeFullName);
		}

		public bool HasCustomAttribute<T>() where T : Attribute => @this.HasCustomAttribute(typeof(T).FullName!);
	}
}


