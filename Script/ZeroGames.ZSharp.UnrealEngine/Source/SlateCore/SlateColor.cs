// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.SlateCore;

public partial class FSlateColor
{

    public FSlateColor(float r, float g, float b, float a = 1)
    {
        SpecifiedColor = new(r, g, b, a);
    }

    public FSlateColor(FLinearColor color)
    {
        SpecifiedColor = color;
    }

    public FSlateColor(FColor color, bool srgb)
    {
        SpecifiedColor = new(color, srgb);
    }
    
    public static FSlateColor White => new(1,1,1);
    public static FSlateColor Gray => new(0.5f,0.5f,0.5f);
    public static FSlateColor Black => new(0,0,0);
    public static FSlateColor Transparent => new(0,0,0,0);
    public static FSlateColor Red => new(1,0,0);
    public static FSlateColor Green => new(0,1,0);
    public static FSlateColor Blue => new(0,0,1);
    public static FSlateColor Cyan => new(0, 1, 1);
    public static FSlateColor Magenta => new(1, 0, 1);
    public static FSlateColor Yellow => new(1,1,0);
    
}


