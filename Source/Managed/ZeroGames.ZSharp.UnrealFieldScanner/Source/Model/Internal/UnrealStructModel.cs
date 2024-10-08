// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal abstract class UnrealStructModel : UnrealFieldModel, IUnrealStructModel
{

	protected UnrealStructModel(string name, ITypeResolver typeResolver, ICustomAttributeProvider attributeProvider) : base(name, typeResolver, attributeProvider){}
	
	public virtual IReadOnlyList<IUnrealPropertyModel> Properties => _properties;
	
	protected readonly List<UnrealPropertyModel> _properties = new();
	
}


