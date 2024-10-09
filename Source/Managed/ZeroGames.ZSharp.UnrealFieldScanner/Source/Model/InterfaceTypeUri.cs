// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public readonly struct InterfaceTypeUri(string assemblyName, string fullName) : ITypeUri
{
	public string AssemblyName { get; } = assemblyName;
	public string FullName { get; } = fullName;
}


