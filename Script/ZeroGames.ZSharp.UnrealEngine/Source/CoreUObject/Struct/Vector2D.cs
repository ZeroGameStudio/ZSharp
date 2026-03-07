// Copyright Zero Games. All Rights Reserved.

using ZeroGames.Extensions.Math;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class FVector2D
{
	
	public static FVector2D MakeForward(double scalar = 1) => new(scalar, 0);
	public static FVector2D MakeBackward(double scalar = 1) => new(-scalar, 0);
	public static FVector2D MakeRight(double scalar = 1) => new(0, scalar);
	public static FVector2D MakeLeft(double scalar = 1) => new(0, -scalar);

	public static double Dot(FVector2D lhs, FVector2D rhs) => lhs | rhs;
	
	public static double Distance(FVector2D lhs, FVector2D rhs) => lhs.Distance(rhs);
	public static double DistanceSquared(FVector2D lhs, FVector2D rhs) => lhs.DistanceSquared(rhs);
	
	public FVector2D(double scalar) : this(scalar, scalar){}
	
	public FVector2D(double x, double y) : this()
	{
		X = x;
		Y = y;
	}
	
	public static implicit operator FVector2D(Vector2 value) => new(value.X, value.Y);
	public static implicit operator Vector2(FVector2D value) => new(value.X, value.Y);
	
	public void Deconstruct(out double x, out double y)
	{
		x = X;
		y = Y;
	}
	
	public override string ToString()
	{
		return $"Vector2D {{ X={X}, Y={Y} }}";
	}

	public double Dot(FVector2D other) => X * other.X + Y * other.Y;
	
	public double Distance(FVector2D other) => Math.Sqrt(DistanceSquared(other));
	public double DistanceSquared(FVector2D other) => (X - other.X) * (X - other.X) + (Y - other.Y) * (Y - other.Y);
	
	public bool IsNearlyEqual(FVector2D other, double tolerance = TOLERANCE) => Math.Abs(X - other.X) <= tolerance && Math.Abs(Y - other.Y) <= tolerance;

	public bool Normalize(double tolerance = NORMALIZED_TOLERANCE)
	{
		double sqrSize = SizeSquared;
		if (sqrSize > TOLERANCE)
		{
			double invSize = 1 / Math.Sqrt(sqrSize);
			X *= invSize;
			Y *= invSize;
			return true;
		}

		return false;
	}
	
	public void Set(double x, double y) => (X, Y) = (x, y);

	public static FVector2D operator+(FVector2D @this) => new(@this.X, @this.Y);
	public static FVector2D operator-(FVector2D @this) => new(-@this.X, -@this.Y);

	public static FVector2D operator+(FVector2D lhs, FVector2D rhs) => new(lhs.X + rhs.X, lhs.Y + rhs.Y);
	public static FVector2D operator+(FVector2D lhs, float rhs) => new(lhs.X + rhs, lhs.Y + rhs);
	public static FVector2D operator+(FVector2D lhs, double rhs) => new(lhs.X + rhs, lhs.Y + rhs);
	public static FVector2D operator+(float lhs, FVector2D rhs) => new(lhs + rhs.X, lhs + rhs.Y);
	public static FVector2D operator+(double lhs, FVector2D rhs) => new(lhs + rhs.X, lhs + rhs.Y);
	
	public static FVector2D operator-(FVector2D lhs, FVector2D rhs) => new(lhs.X - rhs.X, lhs.Y - rhs.Y);
	public static FVector2D operator-(FVector2D lhs, float rhs) => new(lhs.X - rhs, lhs.Y - rhs);
	public static FVector2D operator-(FVector2D lhs, double rhs) => new(lhs.X - rhs, lhs.Y - rhs);
	public static FVector2D operator-(float lhs, FVector2D rhs) => new(lhs - rhs.X, lhs - rhs.Y);
	public static FVector2D operator-(double lhs, FVector2D rhs) => new(lhs - rhs.X, lhs - rhs.Y);

	public static FVector2D operator*(FVector2D lhs, FVector2D rhs) => new(lhs.X * rhs.X, lhs.Y * rhs.Y);
	public static FVector2D operator*(FVector2D lhs, float rhs) => new(lhs.X * rhs, lhs.Y * rhs);
	public static FVector2D operator*(FVector2D lhs, double rhs) => new(lhs.X * rhs, lhs.Y * rhs);
	public static FVector2D operator*(float lhs, FVector2D rhs) => new(lhs * rhs.X, lhs * rhs.Y);
	public static FVector2D operator*(double lhs, FVector2D rhs) => new(lhs * rhs.X, lhs * rhs.Y);
	
	public static FVector2D operator/(FVector2D lhs, FVector2D rhs) => new(lhs.X / rhs.X, lhs.Y / rhs.Y);
	public static FVector2D operator/(FVector2D lhs, float rhs) => new(lhs.X / rhs, lhs.Y / rhs);
	public static FVector2D operator/(FVector2D lhs, double rhs) => new(lhs.X / rhs, lhs.Y / rhs);
	public static FVector2D operator/(float lhs, FVector2D rhs) => new(lhs / rhs.X, lhs / rhs.Y);
	public static FVector2D operator/(double lhs, FVector2D rhs) => new(lhs / rhs.X, lhs / rhs.Y);

	public static double operator|(FVector2D lhs, FVector2D rhs) => lhs.Dot(rhs);
	
	public double this[int32 index]
	{
		get => index switch
		{
			0 => X,
			1 => Y,
			_ => throw new IndexOutOfRangeException()
		};
		set
		{
			if (index is 0)
			{
				X = value;
			}
			else if (index is 1)
			{
				Y = value;
			}
			else
			{
				throw new IndexOutOfRangeException();
			}
		}
	}
	
	public static FVector2D Zero => new();
	public static FVector2D One => new(1.0);
	
	public static FVector2D UnitX => Forward;
	public static FVector2D UnitY => Right;

	public static FVector2D XAxis => Forward;
	public static FVector2D YAxis => Right;

	public static FVector2D Forward => MakeForward();
	public static FVector2D Backward => MakeBackward();
	public static FVector2D Right => MakeRight();
	public static FVector2D Left => MakeLeft();

	public double Max => Math.Max(X, Y);
	public double AbsMax => Math.Max(Math.Abs(X), Math.Abs(Y));
	public double Min => Math.Min(X, Y);
	public double AbsMin => Math.Min(Math.Abs(X), Math.Abs(Y));
	public FVector2D Abs => new(Math.Abs(X), Math.Abs(Y));

	public double Size => Math.Sqrt(SizeSquared);
	public double SizeSquared => X * X + Y * Y;
	public bool IsZero => X == 0 && Y == 0;
	public bool IsNearlyZero => Math.Abs(X) <= TOLERANCE && Math.Abs(Y) <= TOLERANCE;
	public bool AllComponentsEqual => Math.Abs(X - Y) <= TOLERANCE;
	public bool IsNormalized => Math.Abs(SizeSquared - 1) <= TOLERANCE;
	public FVector2D Normalized
	{
		get
		{
			double sqrSize = SizeSquared;
			if (sqrSize > TOLERANCE)
			{
				double invSize = 1 / Math.Sqrt(sqrSize);
				return new(X * invSize, Y * invSize);
			}

			return Zero;
		}
	}
	
	private const double TOLERANCE = 1e-4;
	private const double NORMALIZED_TOLERANCE = 1e-8;
	
}


