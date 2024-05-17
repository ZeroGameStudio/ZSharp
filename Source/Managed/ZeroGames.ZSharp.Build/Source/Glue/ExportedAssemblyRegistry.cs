// Copyright Zero Games. All Rights Reserved.

using System.Collections.Concurrent;
using System.Collections.ObjectModel;

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
		if (Locked)
		{
			return;
		}
		
		Volatile.Write(ref _locked, true);

		Dictionary<string, ExportedAssembly> map = new();
		foreach (var asm in _assemblyMap.Values)
		{
			foreach (var type in asm.ExportedTypes)
			{
				map[type.Module] = asm;
			}
		}

		_moduleAssemblyMap = new(map);
	}

	public ExportedAssembly? GetModuleAssembly(string module)
	{
		if (!Locked)
		{
			throw new InvalidOperationException("Register has not locked.");
		}

		_moduleAssemblyMap!.TryGetValue(module, out var asm);
		return asm;
	}

	public bool Locked => Volatile.Read(ref _locked);
	public IEnumerable<ExportedType> ExportedTypes => _assemblyMap.SelectMany(pair => pair.Value.ExportedTypes);

	private bool _locked;
	private ConcurrentDictionary<string, ExportedAssembly> _assemblyMap = new();
	private ReadOnlyDictionary<string, ExportedAssembly>? _moduleAssemblyMap;

}