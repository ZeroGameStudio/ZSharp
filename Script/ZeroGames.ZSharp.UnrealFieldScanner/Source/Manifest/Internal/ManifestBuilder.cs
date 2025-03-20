// Copyright Zero Games. All Rights Reserved.

using System.Threading;
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
		if (typeModel is IUnrealEnumModel enumModel)
		{
			UnrealEnumDefinition enumDef = ScanEnumModel(enumModel);
			lock (_manifestLock)
			{
				manifest.Enums.Add(enumDef);
			}
		}
		else if (typeModel is IUnrealScriptStructModel scriptStructModel)
		{
			UnrealScriptStructDefinition scriptStructDef = ScanScriptStructModel(scriptStructModel);
			lock (_manifestLock)
			{
				manifest.Structs.Add(scriptStructDef);
			}
		}
		else if (typeModel is IUnrealDelegateModel delegateModel)
		{
			UnrealDelegateDefinition delegateDef = ScanDelegateModel(delegateModel);
			lock (_manifestLock)
			{
				manifest.Delegates.Add(delegateDef);
			}
		}
		else if (typeModel is IUnrealClassModel classModel)
		{
			UnrealClassDefinition classDef = ScanClassModel(classModel);
			lock (_manifestLock)
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
	private readonly Lock _manifestLock = new();

}


