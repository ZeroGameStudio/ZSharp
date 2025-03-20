// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class UnrealFunctionModel : UnrealStructModel, IUnrealFunctionModel
{

	public UnrealFunctionModel(ModelRegistry registry, MethodDefinition methodDef, IUnrealClassModel outer, string? eventOverrideName) : base(methodDef.Name, registry, methodDef)
	{
		Visibility = methodDef.IsPublic ? EMemberVisibility.Public : methodDef.IsFamily ? EMemberVisibility.Protected : EMemberVisibility.Private;
		Outer = outer;
		EventOverrideName = eventOverrideName;

		if (EventOverrideName is null)
		{
			ScanUParams(registry, methodDef);
		}
	}
	
	public EMemberVisibility Visibility { get; }
	public IUnrealClassModel Outer { get; }
	public string? EventOverrideName { get; }

	private void ScanUParams(ModelRegistry registry, MethodDefinition methodDef)
	{
		foreach (var parameter in methodDef.Parameters)
		{
			_properties.Add(new(registry, parameter, this));
		}

		if (methodDef.ReturnType.FullName != typeof(void).FullName)
		{
			_properties.Add(new(registry, methodDef.MethodReturnType, this));
		}
	}
	
}


