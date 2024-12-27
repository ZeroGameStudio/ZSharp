// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

public static class NamespaceHelper
{

	public static SortedSet<string> LootNamespace(ExportedClass cls, bool includeMethodInterface)
	{
		SortedSet<string> res = [];
		LootNamespace(cls.BaseType, res);
		foreach (var implementedInterface in cls.Interfaces)
		{
			LootNamespace(implementedInterface, res);
		}
		
		foreach (var method in cls.Methods)
		{
			if (includeMethodInterface)
			{
				LootNamespace(method.OwnerInterface, res);
			}
			
			foreach (var parameter in method.Parameters)
			{
				LootNamespace(parameter.Type, res);
			}
		}

		foreach (var prop in cls.Properties)
		{
			LootNamespace(prop.Type, res);
		}

		return res;
	}

	public static SortedSet<string> LootNamespace(ExportedDelegate del)
	{
		SortedSet<string> res = [];
		foreach (var parameter in del.Parameters)
		{
			LootNamespace(parameter.Type, res);
		}

		return res;
	}

	private static void LootNamespace(FullyExportedTypeName name, SortedSet<string> result)
	{
		result.Add(name.Namespace);
		result.Add(name.Inner.Namespace);
		result.Add(name.Inner.Inner.Namespace);
		result.Add(name.Outer.Namespace);
		result.Add(name.Outer.Inner.Namespace);
	}
	
}