// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Method)]
public class UFunctionAttribute : FunctionSpecifierBase, IZCallAttribute
{
	string? IZCallAttribute.Name => null;
}


