// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core;

namespace ZeroGames.ZSharp.Emit.Specifier;

[AttributeUsage(AttributeTargets.Method)]
public class UFunctionAttribute : ZCallAttribute
{
	public UFunctionAttribute(){}
	public UFunctionAttribute(string name) : base(name){}
}


