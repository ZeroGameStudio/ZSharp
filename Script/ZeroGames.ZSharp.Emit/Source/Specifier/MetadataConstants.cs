// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public static class MetadataConstants
{

	// Common metadata names
	public const string BlueprintType = nameof(BlueprintType);
	public const string NotBlueprintType = nameof(NotBlueprintType);
	public const string IsBlueprintBase = nameof(IsBlueprintBase);
	public const string BlueprintInternalUseOnly = nameof(BlueprintInternalUseOnly);
	public const string BlueprintInternalUseOnlyHierarchical = nameof(BlueprintInternalUseOnlyHierarchical);
	public const string DisplayName = nameof(DisplayName);
	public const string FriendlyName = nameof(FriendlyName);
	public const string Category = nameof(Category);
	public const string Tooltip = nameof(Tooltip);
	public const string ShortTooltip = nameof(ShortTooltip);
	public const string DevelopmentStatus = nameof(DevelopmentStatus);
	public const string Experimental = nameof(Experimental);
	public const string EarlyAccess = nameof(EarlyAccess);
	
	// Class metadata names
	public const string AdvancedClassDisplay = nameof(AdvancedClassDisplay);
	public const string IsConversionRoot = nameof(IsConversionRoot);
	public const string IgnoreCategoryKeywordsInSubclasses = nameof(IgnoreCategoryKeywordsInSubclasses);
	public const string EditorConfig = nameof(EditorConfig);
	public const string BlueprintSpawnableComponent = nameof(BlueprintSpawnableComponent);
	
	// Function metadata names
	public const string CppDefaultPrefix = "CPP_Default_";
	public const string WorldContext = nameof(WorldContext);
	public const string DefaultToSelf = nameof(DefaultToSelf);
	
	// Property metadata names
	public const string EditInline = nameof(EditInline);
	public const string BlueprintGetter = nameof(BlueprintGetter);
	public const string BlueprintSetter = nameof(BlueprintSetter);
	public const string AllowPrivateAccess = nameof(AllowPrivateAccess);
	public const string EditCondition = nameof(EditCondition);
	public const string EditConditionHides = nameof(EditConditionHides);
	public const string InlineEditConditionToggle = nameof(InlineEditConditionToggle);
	public const string ClampMin = nameof(ClampMin);
	public const string ClampMax = nameof(ClampMax);
	public const string UIMin = nameof(UIMin);
	public const string UIMax = nameof(UIMax);
	public const string GetOptions = nameof(GetOptions);
	public const string BindWidget = nameof(BindWidget);
	public const string BindWidgetOptional = nameof(BindWidgetOptional);
	public const string BindWidgetAnim = nameof(BindWidgetAnim);
	public const string BindWidgetAnimOptional = nameof(BindWidgetAnimOptional);
	public const string IsBindableEvent = nameof(IsBindableEvent);
	
	// Parameter metadata names
	public const string NativeConst = nameof(NativeConst);

}


