// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public static class CoreGlobals
{

	public static unsafe bool IsInGameThread => CoreGlobals_Interop.IsInGameThreadFuncPtr() > 0;

	public static unsafe bool GIsServer => *CoreGlobals_Interop.GIsServerPtr > 0;
	public static unsafe bool GIsClient => *CoreGlobals_Interop.GIsClientPtr > 0;
	public static unsafe bool GIsEditor => *CoreGlobals_Interop.GIsEditorPtr > 0;
	
	public static unsafe uint64 GFrameCounter => *CoreGlobals_Interop.GFrameCounterPtr;

	public static IConfig GConfig { get; internal set; } = null!;

	public const string GEngineIni = Engine;
	public const string GGameIni = Game;
	public const string GInputIni = Input;
	public const string GDeviceProfilesIni = DeviceProfiles;
	public const string GGameUserSettingsIni = GameUserSettings;
	public const string GScalabilityIni = Scalability;
	public const string GRuntimeOptionsIni = RuntimeOptions;
	public const string GInstallBundleIni = InstallBundle;
	public const string GHardwareIni = Hardware;
	public const string GGameplayTagsIni = GameplayTags;
	
	public const string GEditorIni = Editor;
	public const string GEditorPerProjectIni = EditorPerProjectUserSettings;
	public const string GEditorSettingsIni = EditorSettings;
	public const string GEditorKeyBindingsIni = EditorKeyBindings;
	public const string GEditorLayoutIni = EditorLayout;
	public const string GCompatIni = Compat;
	public const string GLightmassIni = Lightmass;
	
	// Migrate from FConfigCacheIni::InitializeConfigSystem().
	private const string Engine = nameof(Engine);
	private const string Game = nameof(Game);
	private const string Input = nameof(Input);
	private const string DeviceProfiles = nameof(DeviceProfiles);
	private const string GameUserSettings = nameof(GameUserSettings);
	private const string Scalability = nameof(Scalability);
	private const string RuntimeOptions = nameof(RuntimeOptions);
	private const string InstallBundle = nameof(InstallBundle);
	private const string Hardware = nameof(Hardware);
	private const string GameplayTags = nameof(GameplayTags);
	
	// Migrate from LoadRemainingConfigFiles().
	private const string Editor = nameof(Editor);
	private const string EditorPerProjectUserSettings = nameof(EditorPerProjectUserSettings);
	private const string EditorSettings = nameof(EditorSettings);
	private const string EditorKeyBindings = nameof(EditorKeyBindings);
	private const string EditorLayout = nameof(EditorLayout);
	private const string Compat = nameof(Compat);
	private const string Lightmass = nameof(Lightmass);

}


