// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Class, AllowMultiple = true)]
public class DontCreateDefaultSubobjectAttribute(string subobject) : Attribute, IUnrealReflectionSpecifier
{
	public string Subobject => subobject;
}


