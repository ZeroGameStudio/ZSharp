// Copyright Zero Games. All Rights Reserved.

using System.Collections.Concurrent;

namespace ZeroGames.ZSharp.Build.Glue;

public class ExportedAssemblyRegistry
{

	public void RegisterAssembly(ExportedAssembly assembly)
	{
		if (Volatile.Read(ref _locked))
		{
			throw new InvalidOperationException("Registry has already locked.");
		}
		
		_assemblyMap[assembly.Name] = assembly;
	}

	public void FinishRegister()
	{
		Volatile.Write(ref _locked, true);
	}

	public IEnumerable<ExportedType> ExportedTypes => _assemblyMap.SelectMany(pair => pair.Value.Classes.AsEnumerable<ExportedType>().Concat(pair.Value.Enums));

	private bool _locked;
	private ConcurrentDictionary<string, ExportedAssembly> _assemblyMap = new();

}