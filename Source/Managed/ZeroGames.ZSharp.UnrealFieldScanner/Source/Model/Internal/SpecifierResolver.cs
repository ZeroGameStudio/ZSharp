// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.Loader;
using Mono.Cecil;
using ICustomAttributeProvider = Mono.Cecil.ICustomAttributeProvider;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal static class SpecifierResolver
{

	public static void Resolve(ModelRegistry modelRegistry, ICustomAttributeProvider attributeProvider, ICollection<IUnrealReflectionSpecifier> specifiers)
	{
		foreach (var attribute in attributeProvider.CustomAttributes)
		{
			TypeReference typeRef = attribute.AttributeType;
			string assemblyName = typeRef.Scope.GetAssemblyName();
			if (!_specifierAssemblies.Contains(assemblyName))
			{
				continue;
			}
			
			TypeDefinition typeDef = modelRegistry.ResolveTypeDefinition(typeRef);
			Assembly assembly = AssemblyLoadContext.Default.Assemblies.Single(asm => asm.GetName().Name == assemblyName);
			verify(assembly.GetType(typeDef.FullName) is var specifierRuntimeType && specifierRuntimeType is not null);

			if (specifierRuntimeType.IsAssignableTo(typeof(IUnrealReflectionSpecifier)))
			{
				specifiers.Add(CreateSpecifier(modelRegistry, attribute, specifierRuntimeType));
			}
		}
	}

	private static IUnrealReflectionSpecifier CreateSpecifier(ModelRegistry modelRegistry, CustomAttribute attribute, Type specifierRuntimeType)
	{
		// Loot constructor parameters.
		List<(int32 Index, TypeReference TypeRef)> typeConstructorArguments = new();
		object?[]? constructorParameters = attribute.HasConstructorArguments ? new object?[attribute.ConstructorArguments.Count] : null;
		if (constructorParameters is not null)
		{
			for (int32 i = 0; i < constructorParameters.Length; ++i)
			{
				CustomAttributeArgument arg = attribute.ConstructorArguments[i];
				if (arg.Value is TypeReference typeArg)
				{
					typeConstructorArguments.Add((i, typeArg));
					constructorParameters[i] = null;
				}
				else
				{
					object? parameter = GetAttributeArgumentValue(arg);
					constructorParameters[i] = parameter;
				}
			}
		}
		
		var specifier = (IUnrealReflectionSpecifier)Activator.CreateInstance(specifierRuntimeType, constructorParameters)!;
		
		// Fill scanner assembly name and full name.
		foreach (var arg in typeConstructorArguments)
		{
			IScanTimeType scanTimeType = modelRegistry.GetOrAddTypeModel(modelRegistry.ResolveTypeDefinition(arg.TypeRef));
			SetScannerPropertyForSpecifier(specifier, arg.Index, scanTimeType);
		}

		// Loot properties and fields.
		if (attribute.HasProperties)
		{
			foreach (var namedArg in attribute.Properties)
			{
				CustomAttributeArgument arg = namedArg.Argument;
				if (arg.Value is TypeReference typeArg)
				{
					IScanTimeType scanTimeType = modelRegistry.GetOrAddTypeModel(modelRegistry.ResolveTypeDefinition(typeArg));
					SetScannerPropertyForSpecifier(specifier, namedArg.Name, scanTimeType);
				}
				else
				{
					specifierRuntimeType.GetProperty(namedArg.Name)!.SetValue(specifier, GetAttributeArgumentValue(arg));
				}
			}
		}

		if (attribute.HasFields)
		{
			foreach (var namedArg in attribute.Fields)
			{
				CustomAttributeArgument arg = namedArg.Argument;
				if (arg.Value is TypeReference typeArg)
				{
					IScanTimeType scanTimeType = modelRegistry.GetOrAddTypeModel(modelRegistry.ResolveTypeDefinition(typeArg));
					SetScannerPropertyForSpecifier(specifier, namedArg.Name, scanTimeType);
				}
				else
				{
					specifierRuntimeType.GetField(namedArg.Name)!.SetValue(specifier, GetAttributeArgumentValue(arg));
				}
			}
		}
		
		return specifier;
	}

	private static void SetScannerPropertyForSpecifier(IUnrealReflectionSpecifier specifier, object key, IScanTimeType value)
	{
		Type specifierRuntimeType = specifier.GetType();
		PropertyInfo? property = specifierRuntimeType.GetProperties(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance).FirstOrDefault(prop => prop.PropertyType.IsAssignableFrom(typeof(IScanTimeType)) && prop.GetCustomAttribute<ScanTimeTypeAttribute>() is {} attribute && (attribute.PropertyName == key as string || attribute.ConstructorArgumentIndex == key as int32?));
		property?.SetValue(specifier, value);
	}
	
	private static object? GetAttributeArgumentValue(CustomAttributeArgument arg)
	{
		while (arg.Value is CustomAttributeArgument compositeValue)
		{
			arg = compositeValue;
		}
		
		if (arg.Value is TypeReference)
		{
			return new NotSupportedException();
		}
		
		if (arg.Type.IsArray)
		{
			TypeReference elementTypeRef = arg.Type.GetElementType();
			string assemblyName = elementTypeRef.Scope.GetAssemblyName();
			Assembly assembly = AssemblyLoadContext.Default.Assemblies.Single(asm => asm.GetName().Name == assemblyName);
			verify(assembly.GetType(elementTypeRef.FullName) is var elementType && elementType is not null);
			
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


