// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public interface IMarshalPointer : IDisposable
{
	static abstract IDisposable ToManaged(IntPtr unmanaged);
}


