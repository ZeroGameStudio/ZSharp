// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

partial class FGuid
{
    public const int32 SIZEOF_GUID = 16;
    
    public static implicit operator FGuid(Guid value)
    {
        unsafe
        {
            FGuid result = new();
            Span<uint8> dest = new((uint8*)result.Unmanaged, SIZEOF_GUID);
            MemoryMarshal.Write(dest, value);
            if (BitConverter.IsLittleEndian)
            {
                Swap(dest);
            }
            
            return result;
        }
    }
    
    public static implicit operator Guid(FGuid value)
    {
        // Ensure value is valid.
        _ = ConjugateHandle.FromConjugate(value);
        
        unsafe
        {
            ReadOnlySpan<uint8> source = new((uint8*)value.Unmanaged, SIZEOF_GUID);
            if (BitConverter.IsLittleEndian)
            {
                Span<uint8> swappedSource = stackalloc uint8[SIZEOF_GUID];
                source.CopyTo(swappedSource);
                Swap(swappedSource);
                
                return MemoryMarshal.Read<Guid>(swappedSource);
            }
            else
            {
                return MemoryMarshal.Read<Guid>(source);
            }
        }
    }
    
    // System.Guid      Little Endian:      3210 5476 89AB CDEF
    // FGuid            Little Endian:      3210 7654 BA98 FEDC
    
    // System.Guid      Big Endian:         0123 4567 89AB CDEF
    // FGuid            Big Endian:         0123 4567 89AB CDEF
    private static void Swap(Span<uint8> dest)
    {
        (dest[4], dest[5], dest[6], dest[7]) = (dest[6], dest[7], dest[4], dest[5]);
        (dest[8], dest[9], dest[10], dest[11]) = (dest[11], dest[10], dest[9], dest[8]);
        (dest[12], dest[13], dest[14], dest[15]) = (dest[15], dest[14], dest[13], dest[12]);
    }
    
}


