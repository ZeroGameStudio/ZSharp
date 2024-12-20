﻿// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.Core;

internal sealed class ZCallResolver_Method(MasterAssemblyLoadContext alc) : IZCallResolver
{
	
	public IZCallDispatcher? Resolve(string name)
	{
		if (!name.StartsWith("nm://"))
		{
			return null;
		}

		string[] paths = name.Substring(5).Split(':');
		if (paths.Length != 3)
		{
			return null;
		}
		
		(string assemblyName, string typeName, string methodName) = (paths[0], paths[1], paths[2]);
		RuntimeTypeLocator locator = new(assemblyName, typeName);
		Type? type = _alc.GetType(ref locator);
		if (type is null)
		{
			return null;
		}

		MethodInfo[] methods = type.GetMethods(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.Instance).Where(method =>
		{
			var attr = (IZCallAttribute?)method.GetCustomAttributes().FirstOrDefault(attr => attr.GetType().IsAssignableTo(typeof(IZCallAttribute)));
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

		return new ZCallDispatcher_Method(name, methods[0]);
	}

	private readonly MasterAssemblyLoadContext _alc = alc;

}


