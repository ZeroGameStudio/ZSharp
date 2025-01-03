﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

internal sealed class ZCallResolver_Property(MasterAssemblyLoadContext alc) : IZCallResolver
{
	
	public IZCallDispatcher? Resolve(string name)
	{
		if (!name.StartsWith("np://"))
		{
			return null;
		}

		throw new NotImplementedException();
	}

	private readonly MasterAssemblyLoadContext _alc = alc;

}


