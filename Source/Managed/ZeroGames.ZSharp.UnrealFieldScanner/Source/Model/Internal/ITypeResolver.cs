// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal interface ITypeResolver
{
	TypeDefinition ResolveTypeDefinition(string assemblyName, string typeFullName);
}

internal static class TypeResolverExtensions
{

	public static TypeDefinition ResolveTypeDefinition(this ITypeResolver resolver, TypeReference typeRef) => resolver.ResolveTypeDefinition(typeRef.Scope.GetAssemblyName(), typeRef.FullName);

}


