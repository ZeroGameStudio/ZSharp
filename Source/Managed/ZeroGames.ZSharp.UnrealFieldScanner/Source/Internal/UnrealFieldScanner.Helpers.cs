// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
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

	private CustomAttribute GetCustomAttribute(ICustomAttributeProvider provider, string attributeFullName) => GetCustomAttributeOrDefault(provider, attributeFullName) ?? throw new InvalidOperationException();

	private CustomAttribute? GetCustomAttributeOrDefault(ICustomAttributeProvider provider, string attributeFullName) => provider.CustomAttributes.SingleOrDefault(attr => attr.AttributeType.FullName == attributeFullName);

	private object? GetAttributeArgValue(CustomAttributeArgument arg)
	{
		// Object type argument.
		if (arg.Type.FullName == typeof(CustomAttributeArgument).FullName)
		{
			arg = (CustomAttributeArgument)arg.Value;
		}
		else if (arg.Type.IsArray)
		{
			var args = (CustomAttributeArgument[])arg.Value;
			if (args.Length == 0)
			{
				return null;
			}

			Type arrType = GetAttributeArgValue(args[0])!.GetType().MakeArrayType();
			Array arr = Array.CreateInstance(arrType, args.Length);
			for (int32 i = 0; i < arr.Length; ++i)
			{
				arr.SetValue(GetAttributeArgValue(args[i]), i);
			}
			
			return arr;
		}

		return arg.Value;
	}

	private T GetAttributeArgValue<T>(CustomAttributeArgument arg) => (T)GetAttributeArgValue(arg)!;

	private T GetAttributeCtorArgValue<T>(CustomAttribute attribute, int32 index) => GetAttributeArgValue<T>(attribute.ConstructorArguments[index]);

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
		CustomAttribute? umetaAttribute = GetCustomAttributeOrDefault(provider, typeof(UMetaAttribute).FullName!);
		if (umetaAttribute is null)
		{
			return null;
		}
		
		string[]? pairs = GetAttributeCtorArgValue<string[]?>(umetaAttribute, 0);
		if (pairs is null)
		{
			return null;
		}

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


