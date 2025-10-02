// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Method, AllowMultiple = true)]
public class DefaultParameterAttribute : FunctionSpecifierBase
{
	public required string Parameter { get; init; }
	public required object Default { get; init; }
}


