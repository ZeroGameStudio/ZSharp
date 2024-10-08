// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class LegacyUnrealFieldScanner
{

	private void ScanUFunctions(TypeDefinition type, UnrealClassDefinition cls)
	{
		foreach (var method in type.Methods)
		{
			if (GetCustomAttributeOrDefault<UFunctionAttribute>(method) is { } ufunctionAttr)
			{
				ScanUFunction(type, method, cls, ufunctionAttr);
			}
		}
	}

	private void ScanUFunction(TypeDefinition type, MethodDefinition method, UnrealClassDefinition cls, CustomAttribute ufunctionAttr)
	{
		TryGetAttributePropertyValue<string>(ufunctionAttr, nameof(UFunctionAttribute.Name), out var name);
		name ??= method.Name;
		string zcallName = $"m://{method.Module.Assembly.Name.Name}:{type.FullName}:{name}";
		UnrealFunctionDefinition def = new()
		{
			Name = name,
			ZCallName = zcallName,
		};

		if (GetCustomAttributeOrDefault<BlueprintEventOverrideAttribute>(method) is { } overrideAttr)
		{
			def.Name = GetAttributePropertyValue<string>(overrideAttr, nameof(BlueprintEventOverrideAttribute.EventName));
			def.FunctionFlags |= EFunctionFlags.FUNC_BlueprintEvent;
		}
		
		if (HasCustomAttribute<BlueprintCallableAttribute>(method))
		{
			def.FunctionFlags |= EFunctionFlags.FUNC_BlueprintCallable;
		}
		else if (HasCustomAttribute<BlueprintPureAttribute>(method))
		{
			def.FunctionFlags |= EFunctionFlags.FUNC_BlueprintCallable;
			def.FunctionFlags |= EFunctionFlags.FUNC_BlueprintPure;
		}
		
		ScanUParams(method, def);
		if (method.ReturnType.FullName != typeof(void).FullName)
		{
			ScanUReturnParam(method.ReturnType, def);
		}
		
		if (_withMetadata)
		{
			ScanMetadata(method, def);
		}

		cls.Functions.Add(def);
	}
	
}


