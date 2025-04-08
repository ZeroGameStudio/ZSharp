// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZArchive_Interop
	{
		static void SaveUInt8(FArchive& ar, uint8 value);
		static uint8 LoadUInt8(FArchive& ar);

		static void SaveUInt16(FArchive& ar, uint16 value);
		static uint16 LoadUInt16(FArchive& ar);

		static void SaveUInt32(FArchive& ar, uint32 value);
		static uint32 LoadUInt32(FArchive& ar);

		static void SaveUInt64(FArchive& ar, uint64 value);
		static uint64 LoadUInt64(FArchive& ar);

		static void SaveInt8(FArchive& ar, int8 value);
		static int8 LoadInt8(FArchive& ar);

		static void SaveInt16(FArchive& ar, int16 value);
		static int16 LoadInt16(FArchive& ar);

		static void SaveInt32(FArchive& ar, int32 value);
		static int32 LoadInt32(FArchive& ar);

		static void SaveInt64(FArchive& ar, int64 value);
		static int64 LoadInt64(FArchive& ar);

		static void SaveFloat(FArchive& ar, float value);
		static float LoadFloat(FArchive& ar);

		static void SaveDouble(FArchive& ar, double value);
		static double LoadDouble(FArchive& ar);

		static void SaveString(FArchive& ar, FZConjugateHandle value);
		static FZConjugateHandle LoadString(FArchive& ar);

		static void SaveAnsiString(FArchive& ar, FZConjugateHandle value);
		static FZConjugateHandle LoadAnsiString(FArchive& ar);

		static void SaveUtf8String(FArchive& ar, FZConjugateHandle value);
		static FZConjugateHandle LoadUtf8String(FArchive& ar);

		static void SaveName(FArchive& ar, FZConjugateHandle value);
		static FZConjugateHandle LoadName(FArchive& ar);

		static void SaveText(FArchive& ar, FZConjugateHandle value);
		static FZConjugateHandle LoadText(FArchive& ar);

		static void SaveObject(FArchive& ar, FZConjugateHandle value);
		static FZConjugateHandle LoadObject(FArchive& ar);

		static void SaveScriptStruct(FArchive& ar, FZConjugateHandle value);
		static FZConjugateHandle LoadScriptStruct(FArchive& ar);
		
		static uint8 NetSaveScriptStruct(FArchive& ar, FZConjugateHandle value, FZConjugateHandle map, uint8& success);
		static uint8 NetLoadScriptStruct(FArchive& ar, FZConjugateHandle& value, FZConjugateHandle map, uint8& success);

		static void SaveSoftObjectPath(FArchive& ar, FZConjugateHandle value);
		static FZConjugateHandle LoadSoftObjectPath(FArchive& ar);

		static uint8 IsSaving(FArchive& ar);
		static uint8 IsLoading(FArchive& ar);

		static int64 Tell(FArchive& ar);
		static void Seek(FArchive& ar, int64 pos);
		static int64 GetTotalSize(FArchive& ar);
	};
}


