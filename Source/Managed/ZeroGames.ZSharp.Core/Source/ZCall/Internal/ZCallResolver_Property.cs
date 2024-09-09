// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

internal class ZCallResolver_Property : IZCallResolver
{
	
	public IZCallDispatcher? Resolve(string name)
	{
		IMasterAssemblyLoadContext? alc = IMasterAssemblyLoadContext.Instance;
		if (alc is null)
		{
			return null;
		}
		
		if (!name.StartsWith("p://"))
		{
			return null;
		}

		throw new NotImplementedException();
	}
	
}


