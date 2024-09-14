// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Method)]
public class BlueprintEventAttribute() : Attribute
{
	public BlueprintEventAttribute(string implementationZCallName) : this() => ImplementationZCallName = implementationZCallName;
	public string? ImplementationZCallName { get; }
}


