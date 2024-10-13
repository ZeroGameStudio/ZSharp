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
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, BlueprintGetterAttribute specifier)
	{
		IUnrealClassModel? classModel = model.Outer as IUnrealClassModel;
		if (classModel is null)
		{
			throw new InvalidOperationException();
		}

		IUnrealFunctionModel getterModel = classModel.Functions.Single(func => func.Name == specifier.AccessorName);
		// @TODO: Check signature
		
		UnrealClassDefinition classDef = (UnrealClassDefinition)def.Outer;
		UnrealFunctionDefinition functionDef = classDef.Functions.Single(func => func.Name == specifier.AccessorName);
		if ((functionDef.FunctionFlags & EFunctionFlags.FUNC_Event) != EFunctionFlags.FUNC_None)
		{
			throw new InvalidOperationException();
		}
		
		def.PropertyFlags |= EPropertyFlags.CPF_BlueprintVisible;
		if (!model.HasSpecifier<BlueprintSetterAttribute>())
		{
			def.PropertyFlags |= EPropertyFlags.CPF_BlueprintReadOnly;
		}
		def.AddMetadata(MetadataConstants.BlueprintGetter, specifier.AccessorName);
		functionDef.AddMetadata(MetadataConstants.BlueprintGetter);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, BlueprintSetterAttribute specifier)
	{
		IUnrealClassModel? classModel = model.Outer as IUnrealClassModel;
		if (classModel is null)
		{
			throw new InvalidOperationException();
		}

		IUnrealFunctionModel getterModel = classModel.Functions.Single(func => func.Name == specifier.AccessorName);
		// @TODO: Check signature
		
		UnrealClassDefinition classDef = (UnrealClassDefinition)def.Outer;
		UnrealFunctionDefinition functionDef = classDef.Functions.Single(func => func.Name == specifier.AccessorName);
		if ((functionDef.FunctionFlags & EFunctionFlags.FUNC_Event) != EFunctionFlags.FUNC_None)
		{
			throw new InvalidOperationException();
		}
		
		def.PropertyFlags |= EPropertyFlags.CPF_BlueprintVisible;
		def.AddMetadata(MetadataConstants.BlueprintSetter, specifier.AccessorName);
		functionDef.AddMetadata(MetadataConstants.BlueprintSetter);
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, BlueprintAssignableDelegateAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_BlueprintAssignable;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, BlueprintCallableDelegateAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_BlueprintCallable;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, BlueprintAuthorityOnlyDelegateAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_BlueprintAuthorityOnly;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, ConfigAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_Config;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, GlobalConfigAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_Config | EPropertyFlags.CPF_GlobalConfig;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, TransientAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_Transient;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, DuplicateTransientAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_DuplicateTransient;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, TextExportTransientAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_TextExportTransient;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, NonPieDuplicatedTransientAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_NonPIEDuplicateTransient;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, ExportAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_ExportObject;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, InstancedAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_ExportObject | EPropertyFlags.CPF_InstancedReference | EPropertyFlags.CPF_PersistentInstance;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, SaveGameAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_SaveGame;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, SkipSerializationAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_SkipSerialization;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, ReplicatedAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_Net;
		if (specifier.RepNotify is not null)
		{
			def.PropertyFlags |= EPropertyFlags.CPF_RepNotify;
			def.RepNotifyName = specifier.RepNotify;
		}

		throw new NotImplementedException();
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, NotReplicatedAttribute specifier)
	{
		bool isStructMember = def.Outer is UnrealScriptStructDefinition;
		bool isRpcParameter = def.Outer is UnrealFunctionDefinition functionDef && (functionDef.FunctionFlags & EFunctionFlags.FUNC_Net) != EFunctionFlags.FUNC_None;
		if (!isStructMember && !isRpcParameter)
		{
			throw new InvalidOperationException();
		}

		def.PropertyFlags |= EPropertyFlags.CPF_RepSkip;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, FieldNotifyAttribute specifier)
	{
		var classDef = def.Outer as UnrealClassDefinition;
		if (classDef is null)
		{
			throw new InvalidOperationException();
		}
		
		classDef.FieldNotifies.Add(def.Name);
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
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, NoClearAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_NoClear;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, EditFixedSizeAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_EditFixedSize;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, InterpAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_Edit | EPropertyFlags.CPF_BlueprintVisible | EPropertyFlags.CPF_Interp;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, NonTransactionalAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_NonTransactional;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, AssetRegistrySearchableAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_AssetRegistrySearchable;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, SimpleDisplayAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_SimpleDisplay;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, AdvancedDisplayAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_AdvancedDisplay;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, EditorConfigAttribute specifier)
	{
		def.AddMetadata(MetadataConstants.EditorConfig);
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
	
}


