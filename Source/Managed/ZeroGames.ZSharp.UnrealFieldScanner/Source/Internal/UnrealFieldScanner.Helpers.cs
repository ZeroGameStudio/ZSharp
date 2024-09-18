// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Threading;
using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class UnrealFieldScanner
{

	private AssemblyDefinition GetTypeAssembly(TypeReference type)
	{
		IMetadataScope scope = type.Scope;
		string assemblyName = scope.MetadataScopeType == MetadataScopeType.AssemblyNameReference ? scope.Name : throw new InvalidOperationException();
		return GetAssemblyDefinition(assemblyName) ?? throw new InvalidOperationException();
	}

	private TypeDefinition GetTypeDefinition(TypeReference typeRef)
	{
		if (typeRef is TypeDefinition typeDef)
		{
			return typeDef;
		}

		AssemblyDefinition assembly = GetTypeAssembly(typeRef);
		return GetTypeDefinition(assembly, typeRef.FullName);
	}

	private TypeDefinition GetTypeDefinition(AssemblyDefinition assembly, string typeFullName) => GetTypeDefinitionOrDefault(assembly, typeFullName) ?? throw new InvalidOperationException();

	private TypeDefinition? GetTypeDefinitionOrDefault(AssemblyDefinition assembly, string typeFullName) => assembly.Modules.SelectMany(module => module.Types).SingleOrDefault(t => t.FullName == typeFullName);

	private CustomAttribute GetCustomAttribute(ICustomAttributeProvider provider, TypeReference attributeTypeRef) => GetCustomAttribute(provider, attributeTypeRef.FullName!);
	private CustomAttribute GetCustomAttribute(ICustomAttributeProvider provider, Type attributeType) => GetCustomAttribute(provider, attributeType.FullName!);
	private CustomAttribute GetCustomAttribute<T>(ICustomAttributeProvider provider) => GetCustomAttribute(provider, typeof(T));
	private CustomAttribute GetCustomAttribute(ICustomAttributeProvider provider, string attributeFullName) => GetCustomAttributeOrDefault(provider, attributeFullName) ?? throw new InvalidOperationException();

	private CustomAttribute? GetCustomAttributeOrDefault(ICustomAttributeProvider provider, TypeReference attributeTypeRef) => GetCustomAttributeOrDefault(provider, attributeTypeRef.FullName!);
	private CustomAttribute? GetCustomAttributeOrDefault(ICustomAttributeProvider provider, Type attributeType) => GetCustomAttributeOrDefault(provider, attributeType.FullName!);
	private CustomAttribute? GetCustomAttributeOrDefault<T>(ICustomAttributeProvider provider) => GetCustomAttributeOrDefault(provider, typeof(T));
	private CustomAttribute? GetCustomAttributeOrDefault(ICustomAttributeProvider provider, string attributeFullName) => provider.CustomAttributes.SingleOrDefault(attr => attr.AttributeType.FullName == attributeFullName);

	private bool HasCustomAttribute(ICustomAttributeProvider provider, TypeReference attributeTypeRef) => HasCustomAttribute(provider, attributeTypeRef.FullName!);
	private bool HasCustomAttribute(ICustomAttributeProvider provider, Type attributeType) => HasCustomAttribute(provider, attributeType.FullName!);
	private bool HasCustomAttribute<T>(ICustomAttributeProvider provider) => HasCustomAttribute(provider, typeof(T));
	private bool HasCustomAttribute(ICustomAttributeProvider provider, string attributeFullName) => GetCustomAttributeOrDefault(provider, attributeFullName) is not null;
	
	private object GetAttributeArgValue(CustomAttributeArgument arg, Type type)
	{
		// Object type argument.
		if (arg.Value is CustomAttributeArgument compositeValue)
		{
			arg = compositeValue;
		}
		else if (arg.Type.IsArray)
		{
			var args = (CustomAttributeArgument[])arg.Value;
			Array arr = Array.CreateInstance(type, args.Length);
			for (int32 i = 0; i < arr.Length; ++i)
			{
				arr.SetValue(GetAttributeArgValue(args[i], type.GetElementType()!), i);
			}
			
			return arr;
		}

		return arg.Value;
	}

	private T GetAttributeArgValue<T>(CustomAttributeArgument arg) where T : notnull => (T)GetAttributeArgValue(arg, typeof(T));

	private bool IsValidAttributeCtorArgIndex(CustomAttribute attribute, int32 index) => index >= 0 && index < attribute.ConstructorArguments.Count;

	private T GetAttributeCtorArgValue<T>(CustomAttribute attribute, int32 index) where T : notnull => GetAttributeArgValue<T>(attribute.ConstructorArguments[index]);

	private bool TryGetAttributeCtorArgValue<T>(CustomAttribute attribute, int32 index, [NotNullWhen(true)] out T? value) where T : notnull
	{
		if (IsValidAttributeCtorArgIndex(attribute, index))
		{
			value = GetAttributeCtorArgValue<T>(attribute, index);
			return true;
		}
		else
		{
			value = default;
			return false;
		}
	}
	
	private bool DoesAttributeHaveProperty(CustomAttribute attribute, string propertyName) => attribute.Properties.Any(prop => prop.Name == propertyName);

	private T GetAttributePropertyValue<T>(CustomAttribute attribute, string propertyName) where T : notnull => GetAttributeArgValue<T>(attribute.Properties.Single(prop => prop.Name == propertyName).Argument);

	private bool TryGetAttributePropertyValue<T>(CustomAttribute attribute, string propertyName, [NotNullWhen(true)] out T? value) where T : notnull
	{
		if (DoesAttributeHaveProperty(attribute, propertyName))
		{
			value = GetAttributePropertyValue<T>(attribute, propertyName);
			return true;
		}
		else
		{
			value = default;
			return false;
		}
	}
	
	private string GetUnrealFieldPath(TypeReference typeRef) => GetUnrealFieldPathOrDefault(typeRef) ?? throw new InvalidOperationException();

	private string? GetUnrealFieldPathOrDefault(TypeReference typeRef)
	{
		TypeDefinition type = GetTypeDefinition(typeRef);
		if (GetCustomAttributeOrDefault(type, UNREAL_FIELD_PATH_ATTRIBUTE_FULL_NAME) is { } unrealFieldPathAttr)
		{
			return GetAttributeCtorArgValue<string>(unrealFieldPathAttr, 0);
		}

		return null;
	}

	private Dictionary<string, string>? GetMetadataMap(ICustomAttributeProvider provider)
	{
		CustomAttribute? umetaAttribute = GetCustomAttributeOrDefault<UMetaAttribute>(provider);
		if (umetaAttribute is null)
		{
			return null;
		}
		
		string[] pairs = GetAttributeCtorArgValue<string[]>(umetaAttribute, 0);
		Dictionary<string, string> map = new(pairs.Length);
		foreach (var pair in pairs)
		{
			string[] kv = pair.Split('=', 2, StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
			map[kv[0]] = kv[1];
		}

		return map;
	}

	private void BlockUntilDebuggerAttached()
	{
		while (!Debugger.IsAttached)
		{
			Thread.Sleep(1000);
			Logger.Warning("Waiting for debugger...");
		}
		Debugger.Break();
	}

	private const string UNREAL_FIELD_PATH_ATTRIBUTE_FULL_NAME = "ZeroGames.ZSharp.UnrealEngine.UnrealFieldPathAttribute";
	
}


