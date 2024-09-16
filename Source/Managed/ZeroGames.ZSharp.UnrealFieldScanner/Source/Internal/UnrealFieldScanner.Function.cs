// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class UnrealFieldScanner
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
		string name = IsValidAttributeCtorArgIndex(ufunctionAttr, 0) ? GetAttributeCtorArgValue<string>(ufunctionAttr, 0) : method.Name;
		string zcallName = $"m://{method.Module.Assembly.Name.Name}:{type.FullName}:{name}";
		UnrealFunctionDefinition def = new()
		{
			Name = name,
			ZCallName = zcallName,
		};

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
		ScanUReturnParam(method.ReturnType, def);

		if (_withMetadata)
		{
			ScanMetadata(method, def);
		}

		cls.Functions.Add(def);
	}
	
}


