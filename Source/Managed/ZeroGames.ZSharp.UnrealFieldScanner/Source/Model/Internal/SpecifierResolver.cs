// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.Loader;
using Mono.Cecil;
using ICustomAttributeProvider = Mono.Cecil.ICustomAttributeProvider;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal static class SpecifierResolver
{

	public static void Resolve(ITypeResolver typeResolver, ICustomAttributeProvider attributeProvider, ICollection<IUnrealReflectionSpecifier> specifiers)
	{
		foreach (var attribute in attributeProvider.CustomAttributes)
		{
			TypeReference typeRef = attribute.AttributeType;
			if (!_specifierAssemblies.Contains(typeRef.Scope.GetAssemblyName()))
			{
				continue;
			}
			
			TypeDefinition typeDef = typeResolver.ResolveTypeDefinition(typeRef);
			if (typeDef.Interfaces.Any(i => i.InterfaceType.FullName == _specifierInterfaceFullName))
			{
				specifiers.Add(CreateSpecifier(attribute, typeDef));
			}
		}
	}

	private static IUnrealReflectionSpecifier CreateSpecifier(CustomAttribute attribute, TypeDefinition typeDef)
	{
		string assemblyName = typeDef.Scope.GetAssemblyName();
		Assembly assembly = AssemblyLoadContext.Default.Assemblies.Single(asm => asm.GetName().Name == assemblyName);
		Type? attributeRuntimeType = assembly.GetType(typeDef.FullName);
		if (attributeRuntimeType is null)
		{
			throw new InvalidOperationException();
		}

		if (!attributeRuntimeType.IsAssignableTo(typeof(IUnrealReflectionSpecifier)))
		{
			throw new InvalidOperationException();
		}

		// Loot constructor parameters
		object?[]? constructorParameters = attribute.HasConstructorArguments ? new object?[attribute.ConstructorArguments.Count] : null;
		if (constructorParameters is not null)
		{
			for (int32 i = 0; i < constructorParameters.Length; ++i)
			{
				object? parameter = GetAttributeArgumentValue(attribute.ConstructorArguments[i]);
				constructorParameters[i] = parameter;
			}
		}
		
		var specifier = (IUnrealReflectionSpecifier)Activator.CreateInstance(attributeRuntimeType, constructorParameters)!;

		// Loot properties and fields
		if (attribute.HasProperties)
		{
			foreach (var namedArg in attribute.Properties)
			{
				object? value = GetAttributeArgumentValue(namedArg.Argument);
				attributeRuntimeType.GetProperty(namedArg.Name)!.SetValue(specifier, value);
			}
		}

		if (attribute.HasFields)
		{
			foreach (var namedArg in attribute.Fields)
			{
				object? value = GetAttributeArgumentValue(namedArg.Argument);
				attributeRuntimeType.GetField(namedArg.Name)!.SetValue(specifier, value);
			}
		}
		
		return specifier;
	}
	
	private static object? GetAttributeArgumentValue(CustomAttributeArgument arg)
	{
		if (arg.Value is CustomAttributeArgument compositeValue)
		{
			arg = compositeValue;
		}
		else if (arg.Type.IsArray)
		{
			TypeReference elementTypeRef = arg.Type.GetElementType();
			string assemblyName = elementTypeRef.Scope.GetAssemblyName();
			Assembly assembly = AssemblyLoadContext.Default.Assemblies.Single(asm => asm.GetName().Name == assemblyName);
			Type? elementType = assembly.GetType(elementTypeRef.FullName);
			if (elementType is null)
			{
				throw new InvalidOperationException();
			}
			
			var args = (CustomAttributeArgument[])arg.Value;
			Array arr = Array.CreateInstance(elementType.MakeArrayType(), args.Length);
			for (int32 i = 0; i < arr.Length; ++i)
			{
				arr.SetValue(GetAttributeArgumentValue(args[i]), i);
			}
			
			return arr;
		}

		return arg.Value;
	}

	private static readonly IReadOnlySet<string> _specifierAssemblies = new HashSet<string>() { "ZeroGames.ZSharp.Core.UnrealEngine", "ZeroGames.ZSharp.Emit" };
	
	private static readonly string _specifierInterfaceFullName = typeof(IUnrealReflectionSpecifier).FullName!;
	
}


