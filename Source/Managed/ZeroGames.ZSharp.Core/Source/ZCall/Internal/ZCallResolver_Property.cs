// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

internal class ZCallResolver_Property(MasterAssemblyLoadContext alc) : IZCallResolver
{
	
	public IZCallDispatcher? Resolve(string name)
	{
		if (!name.StartsWith("p://"))
		{
			return null;
		}

		throw new NotImplementedException();
	}

	private MasterAssemblyLoadContext _alc = alc;

}


