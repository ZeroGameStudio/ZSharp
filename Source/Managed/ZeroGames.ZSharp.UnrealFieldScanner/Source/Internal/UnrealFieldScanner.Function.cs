// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class UnrealFieldScanner
{

	private void ScanUFunctions(TypeDefinition type, UnrealClassDefinition cls)
	{
		foreach (var method in type.Methods)
		{
			if (GetCustomAttributeOrDefault(method, typeof(UFunctionAttribute).FullName!) is { } ufunctionAttr)
			{
				ScanUFunction(type, cls, method, ufunctionAttr);
			}
		}
	}

	private void ScanUFunction(TypeDefinition type, UnrealClassDefinition cls, MethodDefinition method, CustomAttribute ufunctionAttr)
	{
		UnrealFunctionDefinition def = new()
		{
			Name = GetAttributeCtorArgValue<string>(ufunctionAttr, 0),
		};

		def.ZCallName = def.Name;
		
		ScanUParams(method, def);

		if (_withMetadata)
		{
			ScanMetadata(method, def);
		}

		cls.Functions.Add(def);
	}
	
}


