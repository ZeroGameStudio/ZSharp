// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public interface IMarshalPointer : IDisposable
{
	static abstract object ToManaged(IntPtr unmanaged);
}


