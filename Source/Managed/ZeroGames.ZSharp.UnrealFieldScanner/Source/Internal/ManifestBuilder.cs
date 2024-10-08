// Copyright Zero Games. All Rights Reserved.

using System.Threading.Tasks;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal sealed partial class ManifestBuilder
{
	
	public ManifestBuilder(IModelRegistry modelRegistry)
	{
		_modelRegistry = modelRegistry;
	}

	public UnrealFieldManifest Build()
	{
		UnrealFieldManifest result = new() { ModuleName = _modelRegistry.RootModuleName };

		Parallel.ForEachAsync(_modelRegistry.RootTypes, (type, _) => ScanTypeModel(type)).GetAwaiter().GetResult();

		return result;
	}

	private ValueTask ScanTypeModel(ITypeModel typeModel)
	{
		return ValueTask.CompletedTask;
	}

	private readonly IModelRegistry _modelRegistry;

}


