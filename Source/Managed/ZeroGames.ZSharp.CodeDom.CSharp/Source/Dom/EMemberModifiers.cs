// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

[Flags]
public enum EMemberModifiers
{
	None = 0,
	
	Abstract = 1 << 0,
	Virtual = 1 << 1,
	Sealed = 1 << 2,
	Static = 1 << 3,
	Unsafe = 1 << 4,
	Partial = 1 << 5,
}


