// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

/// <summary>
/// Mono.Cecil's model is too complex and confusing with tons of junk information we don't care at all.
/// So first of all, we use Mono.Cecil to scan assembly metadata and transpile it to our own domain model.
/// Then we let Mono.Cecil go and work on our own model, which makes things much easier.
/// </summary>
public interface IModelRegistry
{
	
	ITypeModel GetTypeModel(string fullName);
	bool TryGetTypeModel(string fullName, [NotNullWhen(true)] out ITypeModel? typeModel);
	
	string RootAssemblyName { get; }
	string RootModuleName { get; }
	bool WithMetadata { get; }
	
	IEnumerable<ITypeModel> RootTypes { get; }
	
	bool IsFullyInitialized { get; }

}


