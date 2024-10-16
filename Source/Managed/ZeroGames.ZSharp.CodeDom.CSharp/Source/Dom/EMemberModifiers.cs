// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

[Flags]
public enum EMemberModifiers
{
	None = 0,
	
	Abstract = 1 << 0,
	Sealed = 1 << 1,
	Static = 1 << 2,
	Unsafe = 1 << 3,
	Partial = 1 << 4,
}


