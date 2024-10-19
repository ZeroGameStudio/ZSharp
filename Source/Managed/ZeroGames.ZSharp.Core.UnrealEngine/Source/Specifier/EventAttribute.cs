// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.UnrealEngine.Specifier;

[AttributeUsage(AttributeTargets.Method)]
public class EventImplementationAttribute(string eventName) : Attribute, IUnrealReflectionSpecifier, IZCallAttribute
{
	public string EventName { get; } = eventName;
	string? IZCallAttribute.Name => null;
}


