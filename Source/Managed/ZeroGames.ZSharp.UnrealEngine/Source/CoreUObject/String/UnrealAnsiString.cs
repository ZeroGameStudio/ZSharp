﻿// Copyright Zero Games. All Rights Reserved.

using System.Collections;
using System.Diagnostics.CodeAnalysis;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Text;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(13)]
public sealed class UnrealAnsiString : PlainExportedObjectBase
    , IConjugate<UnrealAnsiString>
    , ICloneable<UnrealAnsiString>
    , IEquatable<UnrealAnsiString>
    , IEquatable<string>
    , IComparable
    , IComparable<UnrealAnsiString>
    , IComparable<string>
    , IComparisonOperators<UnrealAnsiString?, UnrealAnsiString?, bool>
    , IComparisonOperators<UnrealAnsiString?, string?, bool>
    , IEnumerable<char>
    , ISpanParsable<UnrealAnsiString>
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

        internal Enumerator(UnrealAnsiString target)
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
        
        private UnrealAnsiString? _target;
        private readonly string _snapshot;
        private int32 _index = -1;
    }
    
    public static UnrealAnsiString BuildConjugate(IntPtr unmanaged) => new(unmanaged);

    public static UnrealAnsiString Parse(string s, IFormatProvider? provider) => s;

    public static bool TryParse([NotNullWhen(true)] string? s, IFormatProvider? provider, [MaybeNullWhen(false)] out UnrealAnsiString result)
    {
        result = s;
        return s is not null;
    }

    public static UnrealAnsiString Parse(ReadOnlySpan<char> s, IFormatProvider? provider) => Parse(s.ToString(), provider);
    public static bool TryParse(ReadOnlySpan<char> s, IFormatProvider? provider, [MaybeNullWhen(false)] out UnrealAnsiString result) => TryParse(s.ToString(), provider, out result);

    public UnrealAnsiString()
    {
        Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, IntPtr.Zero);
    }
    public UnrealAnsiString(string? content) : this() => Data = content;
    public UnrealAnsiString(UnrealAnsiString? other) : this() => Data = other?.Data;

    public UnrealAnsiString Clone() => new(Data);
    object ICloneable.Clone() => Clone();

    public bool Equals(string? other) => Data == other;
    public bool Equals(UnrealAnsiString? other) => Equals(other?.Data);
    public override bool Equals(object? obj)
    {
        if (ReferenceEquals(this, obj))
        {
            return true;
        }

        if (obj is UnrealAnsiString other)
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

    public int32 CompareTo(UnrealAnsiString? other) => InternalCompare(this, other);
    public int32 CompareTo(string? other) => InternalCompare(this, other);
    int32 IComparable.CompareTo(object? obj)
    {
        if (obj is null)
        {
            return 1;
        }

        if (obj is UnrealAnsiString other)
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

    public static bool operator ==(UnrealAnsiString? lhs, UnrealAnsiString? rhs) => Equals(lhs, rhs);
    public static bool operator !=(UnrealAnsiString? lhs, UnrealAnsiString? rhs) => !Equals(lhs, rhs);
    public static bool operator >(UnrealAnsiString? lhs, UnrealAnsiString? rhs) => InternalCompare(lhs, rhs) > 0;
    public static bool operator >=(UnrealAnsiString? lhs, UnrealAnsiString? rhs) => InternalCompare(lhs, rhs) >= 0;
    public static bool operator <(UnrealAnsiString? lhs, UnrealAnsiString? rhs) => InternalCompare(lhs, rhs) < 0;
    public static bool operator <=(UnrealAnsiString? lhs, UnrealAnsiString? rhs) => InternalCompare(lhs, rhs) <= 0;

    public static bool operator ==(UnrealAnsiString? lhs, string? rhs) => Equals(lhs?.Data, rhs);
    public static bool operator !=(UnrealAnsiString? lhs, string? rhs) => !Equals(lhs?.Data, rhs);
    public static bool operator >(UnrealAnsiString? lhs, string? rhs) => InternalCompare(lhs, rhs) > 0;
    public static bool operator >=(UnrealAnsiString? lhs, string? rhs) => InternalCompare(lhs, rhs) >= 0;
    public static bool operator <(UnrealAnsiString? lhs, string? rhs) => InternalCompare(lhs, rhs) < 0;
    public static bool operator <=(UnrealAnsiString? lhs, string? rhs) => InternalCompare(lhs, rhs) <= 0;

    public static implicit operator UnrealAnsiString(string? value) => new(value);
    public static implicit operator UnrealAnsiString(ReadOnlySpan<char> value) => new(value.ToString());
    public static implicit operator string(UnrealAnsiString? value) => value?.Data ?? string.Empty;
    public static implicit operator ReadOnlySpan<char>(UnrealAnsiString? value) => value?.Data;
    
    public static IEqualityComparer<UnrealAnsiString> DefaultEqualityComparer { get; } = new EqualityComparer();
    public static IComparer<UnrealAnsiString> DefaultRelationalComparer { get; } = new RelationalComparer();
    
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
    
    private sealed class EqualityComparer : IEqualityComparer<UnrealAnsiString>
    {
        public bool Equals(UnrealAnsiString? lhs, UnrealAnsiString? rhs) => lhs == rhs;
        public int32 GetHashCode(UnrealAnsiString obj) => obj.GetHashCode();
    }

    private sealed class RelationalComparer : IComparer<UnrealAnsiString>
    {
        public int32 Compare(UnrealAnsiString? lhs, UnrealAnsiString? rhs) => InternalCompare(lhs, rhs);
    }
    
    private static int32 InternalCompare(string? lhs, string? rhs) => string.Compare(lhs, rhs, StringComparison.Ordinal);
    
    private UnrealAnsiString(IntPtr unmanaged) : base(unmanaged){}

    private unsafe string InternalGetData()
    {
        using InteropString buffer = new();
        UnrealAnsiString_Interop.GetData(ConjugateHandle.FromConjugate(this), buffer.Address);
        return buffer;
    }
    
    private unsafe void InternalSetData(string? value)
    {
        fixed (char* buffer = value)
        {
            UnrealAnsiString_Interop.SetData(ConjugateHandle.FromConjugate(this), buffer);
        }
    }
    
}

