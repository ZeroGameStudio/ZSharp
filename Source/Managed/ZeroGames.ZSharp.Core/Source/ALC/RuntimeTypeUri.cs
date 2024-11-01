// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public readonly struct RuntimeTypeLocator
{
	
	public RuntimeTypeLocator(string assemblyName, string typeName)
	{
		AssemblyName = assemblyName;
		TypeName = typeName;
	}
	
	public unsafe RuntimeTypeLocator(InteropRuntimeTypeUri* locator)
	{
		AssemblyName = new(locator->AssemblyName);
		TypeName = new(locator->TypeName);
		TypeParameters = new RuntimeTypeLocator[locator->NumTypeParameters];
		for (int32 i = 0; i < TypeParameters.Length; ++i)
		{
			TypeParameters[i] = new(locator->TypeParameters + i);
		}
	}
	
	public string AssemblyName { get; }
	public string TypeName { get; }
	public RuntimeTypeLocator[]? TypeParameters { get; }
	
}


