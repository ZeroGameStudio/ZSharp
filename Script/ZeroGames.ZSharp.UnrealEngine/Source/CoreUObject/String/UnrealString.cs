﻿// Copyright Zero Games. All Rights Reserved.

using System.Collections;
using System.Diagnostics.CodeAnalysis;
using System.Numerics;
using System.Runtime.CompilerServices;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(11)]
[ConjugateKey("Unreal.String")]
public sealed class UnrealString : UnrealConjugateBase
    , IConjugate<UnrealString>
    , ICloneable<UnrealString>
    , IEquatable<UnrealString>
    , IEquatable<string>
    , IComparable
    , IComparable<UnrealString>
    , IComparable<string>
    , IComparisonOperators<UnrealString?, UnrealString?, bool>
    , IComparisonOperators<UnrealString?, string?, bool>
    , IEnumerable<char>
    , ISpanParsable<UnrealString>
    , IConvertible
    , IPinnable<char>
    , IUnrealString
{
    
    public struct Enumerator : IEnumerator<char>
    {
        public bool MoveNext()
        {
            string data = GuardInvariant();
            int32 index = _index + 1;
            int32 length = data.Length;
            _index = Math.Min(index, length);
            return _index < length;
        }
        
        public void Reset() => _index = -1;

        public void Dispose() => _target = null;
        
        public char Current
        {
            get
            {
                string data = GuardInvariant();
                return data[_index];
            }
        }
        object IEnumerator.Current => Current;

        internal Enumerator(UnrealString target)
        {
            _target = target;
            _snapshot = target.Data;
        }

        private string GuardInvariant()
        {
            if (_target is null)
            {
                throw new InvalidOperationException("Access enumerator after disposal.");
            }

            if (_target.IsExpired)
            {
                throw new InvalidOperationException("Target enumerable has expired.");
            }

            string data = _target.Data;
            if (data != _snapshot)
            {
                throw new InvalidOperationException("Target enumerable has changed.");
            }

            return data;
        }
        
        private UnrealString? _target;
        private readonly string _snapshot;
        private int32 _index = -1;
    }
    
    public static UnrealString BuildConjugate(IntPtr unmanaged) => new(unmanaged);

    public static UnrealString Parse(string s, IFormatProvider? provider) => s;

    public static bool TryParse([NotNullWhen(true)] string? s, IFormatProvider? provider, [MaybeNullWhen(false)] out UnrealString result)
    {
        result = s;
        return s is not null;
    }

    public static UnrealString Parse(ReadOnlySpan<char> s, IFormatProvider? provider) => Parse(s.ToString(), provider);
    public static bool TryParse(ReadOnlySpan<char> s, IFormatProvider? provider, [MaybeNullWhen(false)] out UnrealString result) => TryParse(s.ToString(), provider, out result);

    public UnrealString() => BuildConjugate_Black(IntPtr.Zero);
    public UnrealString(string? content) : this() => Data = content;
    public UnrealString(UnrealString? other) : this() => Data = other?.Data;

    public UnrealString Clone() => new(Data);
    object ICloneable.Clone() => Clone();

    public bool Equals(string? other) => Data == other;
    public bool Equals(UnrealString? other) => Equals(other?.Data);
    public override bool Equals(object? obj)
    {
        if (ReferenceEquals(this, obj))
        {
            return true;
        }

        if (obj is UnrealString other)
        {
            return Equals(other);
        }

        if (obj is string s)
        {
            return Equals(s);
        }

        return false;
    }

    public override int32 GetHashCode() => Data.GetHashCode();

    public int32 CompareTo(UnrealString? other) => InternalCompare(this, other);
    public int32 CompareTo(string? other) => InternalCompare(this, other);
    int32 IComparable.CompareTo(object? obj)
    {
        if (obj is null)
        {
            return 1;
        }

        if (obj is UnrealString other)
        {
            return CompareTo(other);
        }

        if (obj is string s)
        {
            return CompareTo(s);
        }

        throw new ArgumentException();
    }

    public Enumerator GetEnumerator() => new(this);
    IEnumerator<char> IEnumerable<char>.GetEnumerator() => GetEnumerator();
    IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
    
    public ref readonly char GetPinnableReference() => ref Data.GetPinnableReference();
    
    public override string ToString() => Data;

    public TypeCode GetTypeCode() => Data.GetTypeCode();
    public uint8 ToByte(IFormatProvider? provider = null) => ((IConvertible)Data).ToByte(provider);
    public uint16 ToUInt16(IFormatProvider? provider = null) => ((IConvertible)Data).ToUInt16(provider);
    public uint32 ToUInt32(IFormatProvider? provider = null) => ((IConvertible)Data).ToUInt32(provider);
    public uint64 ToUInt64(IFormatProvider? provider = null) => ((IConvertible)Data).ToUInt64(provider);
    public int8 ToSByte(IFormatProvider? provider = null) => ((IConvertible)Data).ToSByte(provider);
    public int16 ToInt16(IFormatProvider? provider = null) => ((IConvertible)Data).ToInt16(provider);
    public int32 ToInt32(IFormatProvider? provider = null) => ((IConvertible)Data).ToInt32(provider);
    public int64 ToInt64(IFormatProvider? provider = null) => ((IConvertible)Data).ToInt64(provider);
    public decimal ToDecimal(IFormatProvider? provider = null) => ((IConvertible)Data).ToDecimal(provider);
    public float ToSingle(IFormatProvider? provider = null) => ((IConvertible)Data).ToSingle(provider);
    public double ToDouble(IFormatProvider? provider = null) => ((IConvertible)Data).ToDouble(provider);
    public bool ToBoolean(IFormatProvider? provider = null) => ((IConvertible)Data).ToBoolean(provider);
    public char ToChar(IFormatProvider? provider = null) => ((IConvertible)Data).ToChar(provider);
    public string ToString(IFormatProvider? provider) => Data.ToString(provider);
    public DateTime ToDateTime(IFormatProvider? provider = null) => ((IConvertible)Data).ToDateTime(provider);
    public object ToType(Type conversionType, IFormatProvider? provider = null) => ((IConvertible)Data).ToType(conversionType, provider);

    public static bool operator ==(UnrealString? lhs, UnrealString? rhs) => Equals(lhs, rhs);
    public static bool operator !=(UnrealString? lhs, UnrealString? rhs) => !Equals(lhs, rhs);
    public static bool operator >(UnrealString? lhs, UnrealString? rhs) => InternalCompare(lhs, rhs) > 0;
    public static bool operator >=(UnrealString? lhs, UnrealString? rhs) => InternalCompare(lhs, rhs) >= 0;
    public static bool operator <(UnrealString? lhs, UnrealString? rhs) => InternalCompare(lhs, rhs) < 0;
    public static bool operator <=(UnrealString? lhs, UnrealString? rhs) => InternalCompare(lhs, rhs) <= 0;

    public static bool operator ==(UnrealString? lhs, string? rhs) => Equals(lhs?.Data, rhs);
    public static bool operator !=(UnrealString? lhs, string? rhs) => !Equals(lhs?.Data, rhs);
    public static bool operator >(UnrealString? lhs, string? rhs) => InternalCompare(lhs, rhs) > 0;
    public static bool operator >=(UnrealString? lhs, string? rhs) => InternalCompare(lhs, rhs) >= 0;
    public static bool operator <(UnrealString? lhs, string? rhs) => InternalCompare(lhs, rhs) < 0;
    public static bool operator <=(UnrealString? lhs, string? rhs) => InternalCompare(lhs, rhs) <= 0;

    public static implicit operator UnrealString(string? value) => new(value);
    public static implicit operator UnrealString(ReadOnlySpan<char> value) => new(value.ToString());
    public static implicit operator string(UnrealString? value) => value?.Data ?? string.Empty;
    public static implicit operator ReadOnlySpan<char>(UnrealString? value) => value?.Data;
    
    public static IEqualityComparer<UnrealString> DefaultEqualityComparer { get; } = new EqualityComparer();
    public static IComparer<UnrealString> DefaultRelationalComparer { get; } = new RelationalComparer();
    
    [AllowNull]
    public string Data
    {
        get
        {
            MasterAlcCache.GuardInvariant();
            return InternalGetData();
        }
        set
        {
            MasterAlcCache.GuardInvariant();
            InternalSetData(value);
        }
    }
    
    private sealed class EqualityComparer : IEqualityComparer<UnrealString>
    {
        public bool Equals(UnrealString? lhs, UnrealString? rhs) => lhs == rhs;
        public int32 GetHashCode(UnrealString obj) => obj.GetHashCode();
    }

    private sealed class RelationalComparer : IComparer<UnrealString>
    {
        public int32 Compare(UnrealString? lhs, UnrealString? rhs) => InternalCompare(lhs, rhs);
    }
    
    private static int32 InternalCompare(string? lhs, string? rhs) => string.Compare(lhs, rhs, StringComparison.Ordinal);
    
    private UnrealString(IntPtr unmanaged) : base(unmanaged){}

    private unsafe string InternalGetData() => new(UnrealString_Interop.GetData(ConjugateHandle.FromConjugate(this)));
    
    private unsafe void InternalSetData(string? value)
    {
        fixed (char* buffer = value)
        {
            UnrealString_Interop.SetData(ConjugateHandle.FromConjugate(this), buffer);
        }
    }
    
}


