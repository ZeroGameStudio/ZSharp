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
		
		// UHT sorts function by name using StringComparison.OrdinalIgnoreCase strategy. (@see: UhtHeaderCodeGeneratorCppFile.cs)
		classDef.Functions.Sort((lhs, rhs) => StringComparer.OrdinalIgnoreCase.Compare(lhs.Name, rhs.Name));
	}

	private UnrealFunctionDefinition ScanUFunction(UnrealClassDefinition classDef, IUnrealFunctionModel functionModel)
	{
		UnrealFunctionDefinition result = new()
		{
			Outer = classDef,
			FunctionFlags = EFunctionFlags.FUNC_Native,
			Name = functionModel.Name,
			ZCallName = functionModel.Name,
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
		
		FinishScanUFunction(classDef, functionModel, result);
		
		return result;
	}

	private void FinishScanUFunction(UnrealClassDefinition classDef, IUnrealFunctionModel functionModel, UnrealFunctionDefinition result)
	{
		// Only event function can be overridden.
		if (!result.FunctionFlags.HasFlag(EFunctionFlags.FUNC_Event))
		{
			result.FunctionFlags |= EFunctionFlags.FUNC_Final;
		}

		// Check for non-return out param.
		if (result.Properties.Any(p => (p.PropertyFlags & (EPropertyFlags.CPF_OutParm | EPropertyFlags.CPF_ReturnParm)) == EPropertyFlags.CPF_OutParm))
		{
			result.FunctionFlags |= EFunctionFlags.FUNC_HasOutParms;
		}
	}
	
}


