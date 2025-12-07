// Copyright Zero Games. All Rights Reserved.

using ZeroGames.Extensions.Math;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class FVector
{

	public static FVector MakeForward(double scalar = 1) => new(scalar, 0, 0);
	public static FVector MakeBackward(double scalar = 1) => new(-scalar, 0, 0);
	public static FVector MakeRight(double scalar = 1) => new(0, scalar, 0);
	public static FVector MakeLeft(double scalar = 1) => new(0, -scalar, 0);
	public static FVector MakeUp(double scalar = 1) => new(0, 0, scalar);
	public static FVector MakeDown(double scalar = 1) => new(0, 0, -scalar);

	public static double Dot(FVector lhs, FVector rhs) => lhs | rhs;
	public static FVector Cross(FVector lhs, FVector rhs) => lhs ^ rhs;
	
	public static double Distance(FVector lhs, FVector rhs) => lhs.Distance(rhs);
	public static double DistanceSquared(FVector lhs, FVector rhs) => lhs.DistanceSquared(rhs);
	public static double Distance2D(FVector lhs, FVector rhs) => lhs.Distance2D(rhs);
	public static double DistanceSquared2D(FVector lhs, FVector rhs) => lhs.DistanceSquared2D(rhs);
	
	public FVector(double scalar) : this(scalar, scalar, scalar){}
	
	public FVector(double x, double y, double z) : this()
	{
		X = x;
		Y = y;
		Z = z;
	}
	
	public static implicit operator FVector(Vector value) => new(value.X, value.Y, value.Z);
	public static implicit operator Vector(FVector value) => new(value.X, value.Y, value.Z);
	
	public void Deconstruct(out double x, out double y, out double z)
	{
		x = X;
		y = Y;
		z = Z;
	}
	public void Deconstruct(out double x, out double y) => Deconstruct(out x, out y, out _);
	
	public override string ToString()
	{
		return $"Vector {{ X={X}, Y={Y}, Z={Z} }}";
	}

	public double Dot(FVector other) => X * other.X + Y * other.Y + Z * other.Z;
	public FVector Cross(FVector other) => new(Y * other.Z - Z * other.Y, Z * other.X - X * other.Z, X * other.Y - Y * other.X);

	public double Distance(FVector other) => Math.Sqrt(DistanceSquared(other));
	public double DistanceSquared(FVector other) => (X - other.X) * (X - other.X) + (Y - other.Y) * (Y - other.Y) + (Z - other.Z) * (Z - other.Z);
	public double Distance2D(FVector other) => Math.Sqrt(DistanceSquared2D(other));
	public double DistanceSquared2D(FVector other) => (X - other.X) * (X - other.X) + (Y - other.Y) * (Y - other.Y);
	
	public bool IsNearlyEqual(FVector other, double tolerance = TOLERANCE) => Math.Abs(X - other.X) <= tolerance && Math.Abs(Y - other.Y) <= tolerance && Math.Abs(Z - other.Z) <= tolerance;

	public bool Normalize(double tolerance = NORMALIZED_TOLERANCE)
	{
		double sqrSize = SizeSquared;
		if (sqrSize > TOLERANCE)
		{
			double invSize = 1 / Math.Sqrt(sqrSize);
			X *= invSize;
			Y *= invSize;
			Z *= invSize;
			return true;
		}

		return false;
	}
	
	public void Set(double x, double y, double z) => (X, Y, Z) = (x, y, z);

	public static FVector operator+(FVector @this) => new(@this.X, @this.Y, @this.Z);
	public static FVector operator-(FVector @this) => new(-@this.X, -@this.Y, -@this.Z);

	public static FVector operator+(FVector lhs, FVector rhs) => new(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z);
	public static FVector operator+(FVector lhs, float rhs) => new(lhs.X + rhs, lhs.Y + rhs, lhs.Z + rhs);
	public static FVector operator+(FVector lhs, double rhs) => new(lhs.X + rhs, lhs.Y + rhs, lhs.Z + rhs);
	public static FVector operator+(float lhs, FVector rhs) => new(lhs + rhs.X, lhs + rhs.Y, lhs + rhs.Z);
	public static FVector operator+(double lhs, FVector rhs) => new(lhs + rhs.X, lhs + rhs.Y, lhs + rhs.Z);
	
	public static FVector operator-(FVector lhs, FVector rhs) => new(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z);
	public static FVector operator-(FVector lhs, float rhs) => new(lhs.X - rhs, lhs.Y - rhs, lhs.Z - rhs);
	public static FVector operator-(FVector lhs, double rhs) => new(lhs.X - rhs, lhs.Y - rhs, lhs.Z - rhs);
	public static FVector operator-(float lhs, FVector rhs) => new(lhs - rhs.X, lhs - rhs.Y, lhs - rhs.Z);
	public static FVector operator-(double lhs, FVector rhs) => new(lhs - rhs.X, lhs - rhs.Y, lhs - rhs.Z);

	public static FVector operator*(FVector lhs, FVector rhs) => new(lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z);
	public static FVector operator*(FVector lhs, float rhs) => new(lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs);
	public static FVector operator*(FVector lhs, double rhs) => new(lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs);
	public static FVector operator*(float lhs, FVector rhs) => new(lhs * rhs.X, lhs * rhs.Y, lhs * rhs.Z);
	public static FVector operator*(double lhs, FVector rhs) => new(lhs * rhs.X, lhs * rhs.Y, lhs * rhs.Z);
	
	public static FVector operator/(FVector lhs, FVector rhs) => new(lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z);
	public static FVector operator/(FVector lhs, float rhs) => new(lhs.X / rhs, lhs.Y / rhs, lhs.Z / rhs);
	public static FVector operator/(FVector lhs, double rhs) => new(lhs.X / rhs, lhs.Y / rhs, lhs.Z / rhs);
	public static FVector operator/(float lhs, FVector rhs) => new(lhs / rhs.X, lhs / rhs.Y, lhs / rhs.Z);
	public static FVector operator/(double lhs, FVector rhs) => new(lhs / rhs.X, lhs / rhs.Y, lhs / rhs.Z);

	public static double operator|(FVector lhs, FVector rhs) => lhs.Dot(rhs);
	public static FVector operator^(FVector lhs, FVector rhs) => lhs.Cross(rhs);
	
	public double this[int32 index]
	{
		get => index switch
		{
			0 => X,
			1 => Y,
			2 => Z,
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
			else if (index is 2)
			{
				Z = value;
			}
			else
			{
				throw new IndexOutOfRangeException();
			}
		}
	}
	
	public static FVector Zero => new();
	public static FVector One => new(1.0);
	
	public static FVector UnitX => Forward;
	public static FVector UnitY => Right;
	public static FVector UnitZ => Up;

	public static FVector XAxis => Forward;
	public static FVector YAxis => Right;
	public static FVector ZAxis => Up;

	public static FVector Forward => MakeForward();
	public static FVector Backward => MakeBackward();
	public static FVector Right => MakeRight();
	public static FVector Left => MakeLeft();
	public static FVector Up => MakeUp();
	public static FVector Down => MakeDown();

	public double Max => Math.Max(X, Math.Max(Y, Z));
	public double AbsMax => Math.Max(Math.Abs(X), Math.Max(Math.Abs(Y), Math.Abs(Z)));
	public double Min => Math.Min(X, Math.Min(Y, Z));
	public double AbsMin => Math.Min(Math.Abs(X), Math.Min(Math.Abs(Y), Math.Abs(Z)));
	public FVector Abs => new(Math.Abs(X), Math.Abs(Y), Math.Abs(Z));

	public double Size => Math.Sqrt(SizeSquared);
	public double SizeSquared => X * X + Y * Y + Z * Z;
	public double Size2D => Math.Sqrt(SizeSquared2D);
	public double SizeSquared2D => X * X + Y * Y;
	public bool IsZero => X == 0 && Y == 0 && Z == 0;
	public bool IsNearlyZero => Math.Abs(X) <= TOLERANCE && Math.Abs(Y) <= TOLERANCE && Math.Abs(Z) <= TOLERANCE;
	public bool AllComponentsEqual => Math.Abs(X - Y) <= TOLERANCE && Math.Abs(X - Z) <= TOLERANCE && Math.Abs(Y - Z) <= TOLERANCE;
	public bool IsNormalized => Math.Abs(SizeSquared - 1) <= TOLERANCE;
	public FVector Normalized
	{
		get
		{
			double sqrSize = SizeSquared;
			if (sqrSize > TOLERANCE)
			{
				double invSize = 1 / Math.Sqrt(sqrSize);
				return new(X * invSize, Y * invSize, Z * invSize);
			}

			return Zero;
		}
	}

	public FRotator Rotator => UKismetMathLibrary.Conv_VectorToRotator(this);
	public FQuat Quat => Rotator.Quat;

	private const double TOLERANCE = 1e-4;
	private const double NORMALIZED_TOLERANCE = 1e-8;
	
}


