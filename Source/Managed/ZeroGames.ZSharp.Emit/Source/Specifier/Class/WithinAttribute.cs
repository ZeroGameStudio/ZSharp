// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Class)]
public class WithinAttribute(Type type) : Attribute
{
	public Type OuterType => type;
}


