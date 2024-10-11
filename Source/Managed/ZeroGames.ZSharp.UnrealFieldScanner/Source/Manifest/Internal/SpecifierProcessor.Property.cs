// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class SpecifierProcessor
{
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, UPropertyAttribute specifier)
	{
		if (specifier.Default is { } defaultValue)
		{
			if (model.Outer is not IUnrealClassModel)
			{
				throw new InvalidOperationException();
			}

			var classDef = (UnrealClassDefinition)def.Outer;
			classDef.PropertyDefaults.Add(new()
			{
				PropertyChain = def.Name,
				Buffer = defaultValue.ToString()!,
			});
		}
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, BlueprintReadWriteAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_BlueprintVisible;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, BlueprintReadOnlyAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_BlueprintVisible | EPropertyFlags.CPF_BlueprintReadOnly;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, EditAnywhereAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_Edit;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, EditDefaultsOnlyAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_Edit | EPropertyFlags.CPF_DisableEditOnInstance;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, EditInstanceOnlyAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_Edit | EPropertyFlags.CPF_DisableEditOnTemplate;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, VisibleAnywhereAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_Edit | EPropertyFlags.CPF_EditConst;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, VisibleDefaultsOnlyAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_Edit | EPropertyFlags.CPF_EditConst | EPropertyFlags.CPF_DisableEditOnInstance;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, VisibleInstanceOnlyAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_Edit | EPropertyFlags.CPF_EditConst | EPropertyFlags.CPF_DisableEditOnTemplate;
	}
	
	private static void ProcessSpecifier_DefaultSubobject(UnrealPropertyDefinition def, IUnrealPropertyModel model, DefaultSubobjectSpecifierBase specifier, bool optional)
	{
		if (model.Outer is not IUnrealClassModel)
		{
			throw new InvalidOperationException();
		}

		if (model.HasSpecifier<DefaultSubobjectAttribute>(true))
		{
			throw new InvalidOperationException();
		}

		var attachmentSpecifier = model.GetSpecifier<AttachmentAttribute>();
		
		var classDef = (UnrealClassDefinition)def.Outer;
		classDef.DefaultSubobjects.Add(new()
		{
			Name = specifier.Name ?? def.Name,
			ClassPath = model.Type.GetUnrealFieldPath() ?? throw new InvalidOperationException(),
			PropertyName = def.Name,
			IsOptional = optional,
			IsRootComponent = model.HasSpecifier<RootComponentAttribute>(),
			AttachParentDefaultSubobjectName = attachmentSpecifier?.Parent,
			AttachSocketName = attachmentSpecifier?.Socket,
		});
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, DefaultSubobjectAttribute specifier)
	{
		ProcessSpecifier_DefaultSubobject(def, model, specifier, false);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, OptionalDefaultSubobjectAttribute specifier)
	{
		ProcessSpecifier_DefaultSubobject(def, model, specifier, true);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, RootComponentAttribute specifier)
	{
		// Do nothing because DefaultSubobject/OptionalDefaultSubobject specifier will deal with us.
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, AttachmentAttribute specifier)
	{
		// Do nothing because DefaultSubobject/OptionalDefaultSubobject specifier will deal with us.
	}
	
	// Parameter
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, RequiredAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_RequiredParm;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, NotReplicatedAttribute specifier)
	{
		UnrealFunctionDefinition? functionDef = def.Outer as UnrealFunctionDefinition;
		if (functionDef is null || (functionDef.FunctionFlags & EFunctionFlags.FUNC_Net) == EFunctionFlags.FUNC_None)
		{
			throw new InvalidOperationException();
		}

		def.PropertyFlags |= EPropertyFlags.CPF_RepSkip;
	}
	
}


