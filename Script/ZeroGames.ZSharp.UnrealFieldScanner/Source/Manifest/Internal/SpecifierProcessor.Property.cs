﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class SpecifierProcessor
{
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, UPropertyAttribute specifier)
	{
		// Nothing to do.
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
		check(model.Outer is IUnrealClassModel);
		
		var classModel = (IUnrealClassModel)model.Outer;
		IUnrealFunctionModel getterModel = classModel.Functions.Single(func => func.Name == specifier.AccessorName);
		// @TODO: Check signature
		
		UnrealClassDefinition classDef = (UnrealClassDefinition)def.Outer;
		UnrealFunctionDefinition functionDef = classDef.Functions.Single(func => func.Name == specifier.AccessorName);
		check(!functionDef.FunctionFlags.HasFlag(EFunctionFlags.FUNC_Event));
		
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
		check(model.Outer is IUnrealClassModel);
		
		var classModel = (IUnrealClassModel)model.Outer;
		IUnrealFunctionModel setterModel = classModel.Functions.Single(func => func.Name == specifier.AccessorName);
		// @TODO: Check signature
		
		UnrealClassDefinition classDef = (UnrealClassDefinition)def.Outer;
		UnrealFunctionDefinition functionDef = classDef.Functions.Single(func => func.Name == specifier.AccessorName);
		check(!functionDef.FunctionFlags.HasFlag(EFunctionFlags.FUNC_Event));
		
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
		def.AddMetadata(MetadataConstants.EditInline, true);
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
		check(model.Outer is IUnrealClassModel);
		
		var classModel = (IUnrealClassModel)model.Outer;
		def.PropertyFlags |= EPropertyFlags.CPF_Net;
		def.RepCondition = specifier.Condition;
		def.RepNotifyCondition = specifier.NotifyCondition;
		def.IsRepPushBased = specifier.IsPushBased;
		if (specifier.Notify is not null)
		{
			check(classModel.Functions.Any(func => func.Name == specifier.Notify));
			
			def.PropertyFlags |= EPropertyFlags.CPF_RepNotify;
			def.RepNotifyName = specifier.Notify;
		}
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, NotReplicatedAttribute specifier)
	{
		bool isStructMember = def.Outer is UnrealScriptStructDefinition;
		bool isRpcParameter = def.Outer is UnrealFunctionDefinition functionDef && functionDef.FunctionFlags.HasFlag(EFunctionFlags.FUNC_Net);
		check(isStructMember || isRpcParameter);

		def.PropertyFlags |= EPropertyFlags.CPF_RepSkip;
	}
	
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, FieldNotifyAttribute specifier)
	{
		check(def.Outer is UnrealClassDefinition);
		
		var classDef = (UnrealClassDefinition)def.Outer;
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
		check(def.Outer is UnrealClassDefinition);

		var attachmentSpecifier = model.GetSpecifier<AttachmentAttribute>();
		var classDef = (UnrealClassDefinition)def.Outer;
		classDef.DefaultSubobjects.Add(new()
		{
			Name = specifier.Name ?? def.Name,
			ClassPath = model.Type.GetUnrealFieldPath() ?? throw Thrower.NoEntry(),
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

	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, DefaultValueAttribute specifier)
	{
		if (specifier.Value?.ToString() is {} defaultValue && !string.IsNullOrWhiteSpace(defaultValue))
		{
			def.Outer.PropertyDefaults.Add(new()
			{
				PropertyChain = def.Name,
				Buffer = defaultValue,
			});
		}
	}

	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, EnumAsByteAttribute specifier)
	{
		if (def.EnumUnderlyingType == EEnumUnderlyingType.None)
		{
			throw new InvalidOperationException();
		}
		
		def.EnumUnderlyingType = EEnumUnderlyingType.UInt8;
	}
	
	// Parameter
	[SpecifierProcessor]
	private static void ProcessSpecifier(UnrealPropertyDefinition def, IUnrealPropertyModel model, RequiredAttribute specifier)
	{
		def.PropertyFlags |= EPropertyFlags.CPF_RequiredParm;
	}
	
}


