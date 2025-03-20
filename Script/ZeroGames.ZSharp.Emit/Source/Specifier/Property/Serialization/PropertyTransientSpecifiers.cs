// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Emit.Specifier;

public abstract class PropertyTransientSpecifierBase : PropertySpecifierBase;

public class TransientAttribute : PropertyTransientSpecifierBase;
public class DuplicateTransientAttribute : PropertyTransientSpecifierBase;
public class TextExportTransientAttribute : PropertyTransientSpecifierBase;
public class NonPieDuplicatedTransientAttribute : PropertyTransientSpecifierBase;


