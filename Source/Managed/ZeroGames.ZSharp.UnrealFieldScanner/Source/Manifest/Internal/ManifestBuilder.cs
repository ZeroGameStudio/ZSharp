// Copyright Zero Games. All Rights Reserved.

using System.Threading.Tasks;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal sealed partial class ManifestBuilder
{
	
	public ManifestBuilder(IModelRegistry modelRegistry)
	{
		_modelRegistry = modelRegistry;

		ScanSpecifierProcessors();
	}

	public UnrealFieldManifest Build()
	{
		UnrealFieldManifest manifest = new() { ModuleName = _modelRegistry.RootModuleName };

		Parallel.ForEach(_modelRegistry.RootTypes, (type, _) => ScanTypeModel(manifest, type));
		
		return manifest;
	}

	private void ScanTypeModel(UnrealFieldManifest manifest, ITypeModel typeModel)
	{
		if (typeModel is IUnrealClassModel classModel)
		{
			UnrealClassDefinition classDef = ScanClassModel(classModel);
			lock (manifest.Classes)
			{
				manifest.Classes.Add(classDef);
			}
		}
		else
		{
			Thrower.NoEntry();
		}
	}

	private readonly IModelRegistry _modelRegistry;

}


