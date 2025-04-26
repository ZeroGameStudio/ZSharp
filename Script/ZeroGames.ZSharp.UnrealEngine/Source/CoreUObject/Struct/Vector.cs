// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public partial class Vector
{

	public static Vector MakeForward(double scalar = 1) => new(scalar, 0, 0);
	public static Vector MakeBackward(double scalar = 1) => new(-scalar, 0, 0);
	public static Vector MakeRight(double scalar = 1) => new(0, scalar, 0);
	public static Vector MakeLeft(double scalar = 1) => new(0, -scalar, 0);
	public static Vector MakeUp(double scalar = 1) => new(0, scalar, 0);
	public static Vector MakeDown(double scalar = 1) => new(0, -scalar, 0);

	public static double Dot(Vector lhs, Vector rhs) => lhs | rhs;
	public static Vector Cross(Vector lhs, Vector rhs) => lhs ^ rhs;
	
	public static double Distance(Vector lhs, Vector rhs) => lhs.Distance(rhs);
	public static double DistanceSquared(Vector lhs, Vector rhs) => lhs.DistanceSquared(rhs);
	public static double Distance2D(Vector lhs, Vector rhs) => lhs.Distance2D(rhs);
	public static double DistanceSquared2D(Vector lhs, Vector rhs) => lhs.DistanceSquared2D(rhs);
	
	public Vector(double scalar) : this(scalar, scalar, scalar){}
	
	public Vector(double x, double y, double z) : this()
	{
		X = x;
		Y = y;
		Z = z;
	}
	
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

	public double Dot(Vector other) => X * other.X + Y * other.Y + Z * other.Z;
	public Vector Cross(Vector other) => new(Y * other.Z - Z * other.Y, Z * other.X - X * other.Z, X * other.Y - Y * other.X);

	public double Distance(Vector other) => Math.Sqrt(DistanceSquared(other));
	public double DistanceSquared(Vector other) => (X - other.X) * (X - other.X) + (Y - other.Y) * (Y - other.Y) + (Z - other.Z) * (Z - other.Z);
	public double Distance2D(Vector other) => Math.Sqrt(DistanceSquared2D(other));
	public double DistanceSquared2D(Vector other) => (X - other.X) * (X - other.X) + (Y - other.Y) * (Y - other.Y);
	
	public bool IsNearlyEqual(Vector other, double tolerance = TOLERANCE) => Math.Abs(X - other.X) <= tolerance && Math.Abs(Y - other.Y) <= tolerance && Math.Abs(Z - other.Z) <= tolerance;

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

	public static Vector operator+(Vector @this) => new(@this.X, @this.Y, @this.Z);
	public static Vector operator-(Vector @this) => new(-@this.X, -@this.Y, -@this.Z);

	public static Vector operator+(Vector lhs, Vector rhs) => new(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z);
	public static Vector operator+(Vector lhs, float rhs) => new(lhs.X + rhs, lhs.Y + rhs, lhs.Z + rhs);
	public static Vector operator+(Vector lhs, double rhs) => new(lhs.X + rhs, lhs.Y + rhs, lhs.Z + rhs);
	public static Vector operator+(float lhs, Vector rhs) => new(lhs + rhs.X, lhs + rhs.Y, lhs + rhs.Z);
	public static Vector operator+(double lhs, Vector rhs) => new(lhs + rhs.X, lhs + rhs.Y, lhs + rhs.Z);
	
	public static Vector operator-(Vector lhs, Vector rhs) => new(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z);
	public static Vector operator-(Vector lhs, float rhs) => new(lhs.X - rhs, lhs.Y - rhs, lhs.Z - rhs);
	public static Vector operator-(Vector lhs, double rhs) => new(lhs.X - rhs, lhs.Y - rhs, lhs.Z - rhs);
	public static Vector operator-(float lhs, Vector rhs) => new(lhs - rhs.X, lhs - rhs.Y, lhs - rhs.Z);
	public static Vector operator-(double lhs, Vector rhs) => new(lhs - rhs.X, lhs - rhs.Y, lhs - rhs.Z);

	public static Vector operator*(Vector lhs, Vector rhs) => new(lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z);
	public static Vector operator*(Vector lhs, float rhs) => new(lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs);
	public static Vector operator*(Vector lhs, double rhs) => new(lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs);
	public static Vector operator*(float lhs, Vector rhs) => new(lhs * rhs.X, lhs * rhs.Y, lhs * rhs.Z);
	public static Vector operator*(double lhs, Vector rhs) => new(lhs * rhs.X, lhs * rhs.Y, lhs * rhs.Z);
	
	public static Vector operator/(Vector lhs, Vector rhs) => new(lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z);
	public static Vector operator/(Vector lhs, float rhs) => new(lhs.X / rhs, lhs.Y / rhs, lhs.Z / rhs);
	public static Vector operator/(Vector lhs, double rhs) => new(lhs.X / rhs, lhs.Y / rhs, lhs.Z / rhs);
	public static Vector operator/(float lhs, Vector rhs) => new(lhs / rhs.X, lhs / rhs.Y, lhs / rhs.Z);
	public static Vector operator/(double lhs, Vector rhs) => new(lhs / rhs.X, lhs / rhs.Y, lhs / rhs.Z);

	public static double operator|(Vector lhs, Vector rhs) => lhs.Dot(rhs);
	public static Vector operator^(Vector lhs, Vector rhs) => lhs.Cross(rhs);
	
	public static explicit operator Rotator(Vector @this) => @this.Rotator;
	public static explicit operator Quat(Vector @this) => @this.Quat;

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
	
	public static Vector Zero => new();
	public static Vector One => new(1.0);
	
	public static Vector UnitX => Forward;
	public static Vector UnitY => Right;
	public static Vector UnitZ => Up;

	public static Vector XAxis => Forward;
	public static Vector YAxis => Right;
	public static Vector ZAxis => Up;

	public static Vector Forward => MakeForward();
	public static Vector Backward => MakeBackward();
	public static Vector Right => MakeRight();
	public static Vector Left => MakeLeft();
	public static Vector Up => MakeUp();
	public static Vector Down => MakeDown();

	public double Max => Math.Max(X, Math.Max(Y, Z));
	public double AbsMax => Math.Max(Math.Abs(X), Math.Max(Math.Abs(Y), Math.Abs(Z)));
	public double Min => Math.Min(X, Math.Min(Y, Z));
	public double AbsMin => Math.Min(Math.Abs(X), Math.Min(Math.Abs(Y), Math.Abs(Z)));
	public Vector Abs => new(Math.Abs(X), Math.Abs(Y), Math.Abs(Z));

	public double Size => Math.Sqrt(SizeSquared);
	public double SizeSquared => X * X + Y * Y + Z * Z;
	public double Size2D => Math.Sqrt(SizeSquared2D);
	public double SizeSquared2D => X * X + Y * Y;
	public bool IsZero => X == 0 && Y == 0 && Z == 0;
	public bool IsNearlyZero => Math.Abs(X) <= TOLERANCE && Math.Abs(Y) <= TOLERANCE && Math.Abs(Z) <= TOLERANCE;
	public bool AllComponentsEqual => Math.Abs(X - Y) <= TOLERANCE && Math.Abs(X - Z) <= TOLERANCE && Math.Abs(Y - Z) <= TOLERANCE;
	public bool IsNormalized => Math.Abs(SizeSquared - 1) <= TOLERANCE;
	public Vector Normalized
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

	public Rotator Rotator => KismetMathLibrary.Conv_VectorToRotator(this);
	public Quat Quat => Rotator.Quat;

	private const double TOLERANCE = 1e-4;
	private const double NORMALIZED_TOLERANCE = 1e-2;
	
}


