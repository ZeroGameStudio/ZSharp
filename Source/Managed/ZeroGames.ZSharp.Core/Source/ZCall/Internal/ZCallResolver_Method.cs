// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.Core;

internal class ZCallResolver_Method : IZCallResolver
{
	
	public IZCallDispatcher? Resolve(string name)
	{
		IMasterAssemblyLoadContext? alc = IMasterAssemblyLoadContext.Instance;
		if (alc is null)
		{
			return null;
		}
		
		if (!name.StartsWith("m://"))
		{
			return null;
		}

		string[] paths = name.Substring(4).Split(':');
		if (paths.Length != 3)
		{
			return null;
		}
		
		(string assemblyName, string typeName, string methodName) = (paths[0], paths[1], paths[2]);
		RuntimeTypeLocator locator = new(assemblyName, typeName);
		Type? type = alc.GetType(ref locator);
		if (type is null)
		{
			return null;
		}

		MethodInfo[] methods = type.GetMethods().Where(method =>
		{
			ZCallAttribute? attr = method.GetCustomAttribute<ZCallAttribute>();
			if (attr is null)
			{
				return false;
			}

			bool explicitNameMatch = attr.Name == methodName;
			bool defaultNameMatch = string.IsNullOrWhiteSpace(attr.Name) && method.Name == methodName;
			return explicitNameMatch || defaultNameMatch;
		}).ToArray();
		if (methods.Length == 0)
		{
			return null;
		}
		else if (methods.Length > 1)
		{
			return null;
		}

		return new ZCallDispatcher_Method { Name = name, Method = methods[0] };
	}
	
}


