// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ManifestBuilder
{

	private void ScanUFunctions(UnrealClassDefinition classDef, IUnrealClassModel classModel)
	{
		foreach (var functionModel in classModel.Functions)
		{
			classDef.Functions.Add(ScanUFunction(classDef, functionModel));
		}
	}

	private UnrealFunctionDefinition ScanUFunction(UnrealClassDefinition classDef, IUnrealFunctionModel functionModel)
	{
		UnrealFunctionDefinition result = new()
		{
			Outer = classDef,
			FunctionFlags = EFunctionFlags.FUNC_Native,
			Name = functionModel.Name,
			ZCallName = $"nm://{functionModel.Outer.AssemblyName}:{functionModel.Outer.FullName}:{functionModel.Name}",
		};
		
		// Early out for event override because what we need is only a flag.
		if (functionModel.EventOverrideName is not null)
		{
			result.Name = functionModel.EventOverrideName;
			result.IsEventOverride = true;
			return result;
		}

		result.FunctionFlags |= functionModel.Visibility switch
		{
			EMemberVisibility.Public => EFunctionFlags.FUNC_Public,
			EMemberVisibility.Protected => EFunctionFlags.FUNC_Protected,
			EMemberVisibility.Private => EFunctionFlags.FUNC_Private,
			_ => throw Thrower.NoEntry()
		};

		ProcessSpecifiers(result, functionModel);
		
		ScanUProperties(result, functionModel);

		foreach (var property in result.Properties)
		{
			if ((property.PropertyFlags & EPropertyFlags.CPF_OutParm) != EPropertyFlags.CPF_None)
			{
				result.FunctionFlags |= EFunctionFlags.FUNC_HasOutParms;
				break;
			}
		}
		
		return result;
	}
	
}


