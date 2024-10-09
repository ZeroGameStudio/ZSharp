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
			Name = functionModel.Name,
			ZCallName = $"m://{functionModel.Outer.AssemblyName}:{functionModel.Outer.FullName}:{functionModel.Name}",
		};

		result.FunctionFlags |= functionModel.Visibility switch
		{
			EMemberVisibility.Public => EFunctionFlags.FUNC_Public,
			EMemberVisibility.Protected => EFunctionFlags.FUNC_Protected,
			EMemberVisibility.Private => EFunctionFlags.FUNC_Private,
			_ => throw new InvalidOperationException(),
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


