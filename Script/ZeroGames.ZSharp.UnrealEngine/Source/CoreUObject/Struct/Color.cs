// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class FColor
{
    
    public FColor(uint8 r, uint8 g, uint8 b, uint8 a = 255) : this()
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }

    public static FColor White => new(255, 255, 255);
    public static FColor Black => new(0, 0, 0);
    public static FColor Transparent => new(0, 0, 0, 0);
    public static FColor Red => new(255, 0, 0);
    public static FColor Green => new(0, 255, 0);
    public static FColor Blue => new(0, 0, 255);
    public static FColor Cyan => new(0, 255, 255);
    public static FColor Magenta => new(255, 0, 255);
    public static FColor Yellow => new(255, 255, 0);
    public static FColor Orange => new(243, 156, 18);
    public static FColor Purple => new(169, 7, 228);
    public static FColor Turquoise => new(26, 188, 156);
    public static FColor Silver => new(189, 195, 199);
    public static FColor Emerald => new(46, 204, 113);

}


