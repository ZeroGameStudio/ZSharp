// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public readonly struct InterfaceTypeUri(string assemblyName, string fullName, string? unrealFieldPath = null) : ITypeUri
{
	public string AssemblyName { get; } = assemblyName;
	public string FullName { get; } = fullName;
	public string UnrealFieldPath { get; } = unrealFieldPath ?? string.Empty;
}


