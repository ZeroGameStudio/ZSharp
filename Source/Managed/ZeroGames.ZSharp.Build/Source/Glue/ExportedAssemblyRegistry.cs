// Copyright Zero Games. All Rights Reserved.

using System.Collections.Concurrent;
using System.Collections.ObjectModel;

namespace ZeroGames.ZSharp.Build.Glue;

public class ExportedAssemblyRegistry
{

	public void RegisterAssembly(ExportedAssembly assembly)
	{
		if (Volatile.Read(ref _isLocked))
		{
			throw new InvalidOperationException("Registry has already locked.");
		}
		
		_assemblyMap[assembly.Name] = assembly;
	}

	public void FinishRegister()
	{
		if (IsLocked)
		{
			return;
		}
		
		Volatile.Write(ref _isLocked, true);

		Dictionary<string, ExportedAssembly> map = new();
		foreach (var asm in _assemblyMap.Values)
		{
			foreach (var type in asm.ExportedTypes)
			{
				map[type.Module] = asm;
			}
		}

		_moduleAssemblyMap = map;
	}

	public ExportedAssembly? GetModuleAssembly(string module)
	{
		if (!IsLocked)
		{
			throw new InvalidOperationException("Register has not locked.");
		}

		_moduleAssemblyMap!.TryGetValue(module, out var asm);
		return asm;
	}

	public bool IsLocked => Volatile.Read(ref _isLocked);
	public IEnumerable<ExportedType> ExportedTypes => _assemblyMap.SelectMany(pair => pair.Value.ExportedTypes);

	private bool _isLocked;
	private ConcurrentDictionary<string, ExportedAssembly> _assemblyMap = new();
	private IReadOnlyDictionary<string, ExportedAssembly>? _moduleAssemblyMap;

}