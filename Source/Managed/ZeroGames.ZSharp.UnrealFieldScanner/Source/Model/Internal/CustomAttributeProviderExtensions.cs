// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public static class CustomAttributeProviderExtensions
{

	public static bool HasCustomAttribute(this ICustomAttributeProvider provider, string attributeTypeFullName)
	{
		return provider.CustomAttributes.Any(attr => attr.AttributeType.FullName == attributeTypeFullName);
	}

	public static bool HasCustomAttribute<T>(this ICustomAttributeProvider provider) where T : Attribute => HasCustomAttribute(provider, typeof(T).FullName!);

}


