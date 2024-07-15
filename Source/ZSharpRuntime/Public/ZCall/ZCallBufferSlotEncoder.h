// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_Array.h"
#include "Conjugate/ZConjugateRegistry_Delegate.h"
#include "Conjugate/ZConjugateRegistry_MulticastInlineDelegate.h"
#include "Conjugate/ZRegularConjugateRegistries.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Conjugate/ZConjugateRegistry_UScriptStruct.h"
#include "Conjugate/ZObjectWrapperConjugateRegisties.h"
#include "Reflection/Wrapper/ZSelfDescriptiveObjectWrappers.h"
#include "ZCall/ZCallBuffer.h"

namespace ZSharp
{

	template <typename T>
	concept CZIsDecayed = !std::is_reference_v<T> && !std::is_const_v<T> && !std::is_volatile_v<T>;
	
	template <CZIsDecayed, typename = void>
	struct TZCallBufferSlotEncoder;

	template <CZIsDecayed T>
	struct TZCallBufferSlotEncoder<T, std::enable_if_t<TPointerIsConvertibleFromTo<T, UObjectBase>::Value>>
	{
		using DecodedType = T*;
		static DecodedType DecodePointer(const FZCallBufferSlot& slot) { return Cast<T>(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(slot.ReadConjugate())); }
	};

	template <CZIsDecayed T>
	struct TZCallBufferSlotEncoder<T*, std::enable_if_t<TPointerIsConvertibleFromTo<T, UObjectBase>::Value>>
	{
		using DecodedType = T*;
		static void Encode(DecodedType value, FZCallBufferSlot& slot) { slot.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(value)); }
		static void EncodeRet(DecodedType value, FZCallBufferSlot& slot) { Encode(value, slot); }
		static DecodedType Decode(const FZCallBufferSlot& slot) { return Cast<T>(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(slot.ReadConjugate())); }
	};

	template <CZIsDecayed T>
	struct TZCallBufferSlotEncoder<const T*, std::enable_if_t<TPointerIsConvertibleFromTo<T, UObjectBase>::Value>>
	{
		using DecodedType = const T*;
		static void Encode(DecodedType value, FZCallBufferSlot& slot) { slot.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(value)); }
		static void EncodeRet(DecodedType value, FZCallBufferSlot& slot) { Encode(value, slot); }
		static DecodedType Decode(const FZCallBufferSlot& slot) { return Cast<T>(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(slot.ReadConjugate())); }
	};

	template <CZIsDecayed T>
	struct TZCallBufferSlotEncoder<T, std::enable_if_t<TZIsUScriptStruct_V<T>>>
	{
		using DecodedType = T;
		static void Encode(const DecodedType& value, FZCallBufferSlot& slot) { slot.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(&value)); }
		static void EncodeRet(const DecodedType& value, FZCallBufferSlot& slot)
		{
			const UScriptStruct* scriptStruct = TBaseStructure<T>::Get();
			slot.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(scriptStruct, [value, scriptStruct](FZSelfDescriptiveScriptStruct& sdss){ scriptStruct->CopyScriptStruct(sdss.GetUnderlyingInstance(), &value); }));
		}
		static DecodedType& Decode(const FZCallBufferSlot& slot)
		{
			const FZSelfDescriptiveScriptStruct* sdss = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(slot.ReadConjugate());
			return *sdss->GetUnderlyingInstance<T>();
		}
		static DecodedType* DecodePointer(const FZCallBufferSlot& slot)
		{
			const FZSelfDescriptiveScriptStruct* sdss = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(slot.ReadConjugate());
			return sdss->GetUnderlyingInstance<T>();
		}
	};

	template <>
	struct TZCallBufferSlotEncoder<TCHAR*>
	{
		using DecodedType = const TCHAR*;
		static void Encode(DecodedType value, FZCallBufferSlot& slot) { slot.WritePointer(value); }
		static void EncodeRet(DecodedType value, FZCallBufferSlot& slot) { Encode(value, slot); }
		static DecodedType Decode(const FZCallBufferSlot& slot) { return static_cast<DecodedType>(slot.ReadPointer()); }
	};

	template <>
	struct TZCallBufferSlotEncoder<const TCHAR*>
	{
		using DecodedType = const TCHAR*;
		static void Encode(DecodedType value, FZCallBufferSlot& slot) { slot.WritePointer(value); }
		static void EncodeRet(DecodedType value, FZCallBufferSlot& slot) { Encode(value, slot); }
		static DecodedType Decode(const FZCallBufferSlot& slot) { return static_cast<DecodedType>(slot.ReadPointer()); }
	};

	template <>
	struct TZCallBufferSlotEncoder<uint8>
	{
		using DecodedType = uint8;
		static void Encode(DecodedType value, FZCallBufferSlot& slot) { slot.WriteUInt8(value); }
		static void EncodeRet(DecodedType value, FZCallBufferSlot& slot) { Encode(value, slot); }
		static DecodedType Decode(const FZCallBufferSlot& slot) { return slot.ReadUInt8(); }
	};

	template <>
	struct TZCallBufferSlotEncoder<uint16>
	{
		using DecodedType = uint16;
		static void Encode(DecodedType value, FZCallBufferSlot& slot) { slot.WriteUInt16(value); }
		static void EncodeRet(DecodedType value, FZCallBufferSlot& slot) { Encode(value, slot); }
		static DecodedType Decode(const FZCallBufferSlot& slot) { return slot.ReadUInt16(); }
	};

	template <>
	struct TZCallBufferSlotEncoder<uint32>
	{
		using DecodedType = uint32;
		static void Encode(DecodedType value, FZCallBufferSlot& slot) { slot.WriteUInt32(value); }
		static void EncodeRet(DecodedType value, FZCallBufferSlot& slot) { Encode(value, slot); }
		static DecodedType Decode(const FZCallBufferSlot& slot) { return slot.ReadUInt32(); }
	};

	template <>
	struct TZCallBufferSlotEncoder<uint64>
	{
		using DecodedType = uint64;
		static void Encode(DecodedType value, FZCallBufferSlot& slot) { slot.WriteUInt64(value); }
		static void EncodeRet(DecodedType value, FZCallBufferSlot& slot) { Encode(value, slot); }
		static DecodedType Decode(const FZCallBufferSlot& slot) { return slot.ReadUInt64(); }
	};

	template <>
	struct TZCallBufferSlotEncoder<int8>
	{
		using DecodedType = int8;
		static void Encode(DecodedType value, FZCallBufferSlot& slot) { slot.WriteInt8(value); }
		static void EncodeRet(DecodedType value, FZCallBufferSlot& slot) { Encode(value, slot); }
		static DecodedType Decode(const FZCallBufferSlot& slot) { return slot.ReadInt8(); }
	};

	template <>
	struct TZCallBufferSlotEncoder<int16>
	{
		using DecodedType = int16;
		static void Encode(DecodedType value, FZCallBufferSlot& slot) { slot.WriteInt16(value); }
		static void EncodeRet(DecodedType value, FZCallBufferSlot& slot) { Encode(value, slot); }
		static DecodedType Decode(const FZCallBufferSlot& slot) { return slot.ReadInt16(); }
	};

	template <>
	struct TZCallBufferSlotEncoder<int32>
	{
		using DecodedType = int32;
		static void Encode(DecodedType value, FZCallBufferSlot& slot) { slot.WriteInt32(value); }
		static void EncodeRet(DecodedType value, FZCallBufferSlot& slot) { Encode(value, slot); }
		static DecodedType Decode(const FZCallBufferSlot& slot) { return slot.ReadInt32(); }
	};

	template <>
	struct TZCallBufferSlotEncoder<int64>
	{
		using DecodedType = int64;
		static void Encode(DecodedType value, FZCallBufferSlot& slot) { slot.WriteInt64(value); }
		static void EncodeRet(DecodedType value, FZCallBufferSlot& slot) { Encode(value, slot); }
		static DecodedType Decode(const FZCallBufferSlot& slot) { return slot.ReadInt64(); }
	};

	template <>
	struct TZCallBufferSlotEncoder<float>
	{
		using DecodedType = float;
		static void Encode(DecodedType value, FZCallBufferSlot& slot) { slot.WriteFloat(value); }
		static void EncodeRet(DecodedType value, FZCallBufferSlot& slot) { Encode(value, slot); }
		static DecodedType Decode(const FZCallBufferSlot& slot) { return slot.ReadFloat(); }
	};

	template <>
	struct TZCallBufferSlotEncoder<double>
	{
		using DecodedType = double;
		static void Encode(DecodedType value, FZCallBufferSlot& slot) { slot.WriteDouble(value); }
		static void EncodeRet(DecodedType value, FZCallBufferSlot& slot) { Encode(value, slot); }
		static DecodedType Decode(const FZCallBufferSlot& slot) { return slot.ReadDouble(); }
	};

	template <>
	struct TZCallBufferSlotEncoder<bool>
	{
		using DecodedType = bool;
		static void Encode(DecodedType value, FZCallBufferSlot& slot) { slot.WriteBool(value); }
		static void EncodeRet(DecodedType value, FZCallBufferSlot& slot) { Encode(value, slot); }
		static DecodedType Decode(const FZCallBufferSlot& slot) { return slot.ReadBool(); }
	};
	
	template <>
	struct TZCallBufferSlotEncoder<FZGCHandle>
	{
		using DecodedType = FZGCHandle;
		static void Encode(DecodedType value, FZCallBufferSlot& slot) { slot.WriteGCHandle(value); }
		static void EncodeRet(DecodedType value, FZCallBufferSlot& slot) { Encode(value, slot); }
		static DecodedType Decode(const FZCallBufferSlot& slot) { return slot.ReadGCHandle(); }
	};

#define IMPLEMENT_REGULAR_ENCODER(Type) \
	template <> \
	struct TZCallBufferSlotEncoder<F##Type> \
	{ \
		using DecodedType = F##Type; \
		static void Encode(const DecodedType& value, FZCallBufferSlot& slot) { slot.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_##Type>().Conjugate(&value, false)); } \
		static void EncodeRet(const DecodedType& value, FZCallBufferSlot& slot) { slot.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_##Type>().Conjugate(new F##Type { value }, true)); } \
		static DecodedType& Decode(const FZCallBufferSlot& slot) { return *IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_##Type>().Conjugate(slot.ReadConjugate()); } \
		static DecodedType* DecodePointer(const FZCallBufferSlot& slot) { return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_##Type>().Conjugate(slot.ReadConjugate()); } \
	};

	IMPLEMENT_REGULAR_ENCODER(String)
	IMPLEMENT_REGULAR_ENCODER(Name)
	IMPLEMENT_REGULAR_ENCODER(Text)

	IMPLEMENT_REGULAR_ENCODER(FieldPath)

#undef IMPLEMENT_REGULAR_ENCODER

#define IMPLEMENT_SELF_DESCRIPTIVE_ENCODER(Type, ConjugateRegistryName) \
	template <> \
	struct TZCallBufferSlotEncoder<FZSelfDescriptive##Type> \
	{ \
		using DecodedType = FZSelfDescriptive##Type; \
		static DecodedType& Decode(const FZCallBufferSlot& slot) { return *IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_##ConjugateRegistryName>().Conjugate(slot.ReadConjugate()); } \
		static DecodedType* DecodePointer(const FZCallBufferSlot& slot) { return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_##ConjugateRegistryName>().Conjugate(slot.ReadConjugate()); } \
	};

#define IMPLEMENT_OBJECT_WRAPPER_ENCODER(Type) IMPLEMENT_SELF_DESCRIPTIVE_ENCODER(Type, Type)

	IMPLEMENT_OBJECT_WRAPPER_ENCODER(SubclassOf)
	IMPLEMENT_OBJECT_WRAPPER_ENCODER(SoftClassPtr)
	IMPLEMENT_OBJECT_WRAPPER_ENCODER(SoftObjectPtr)
	IMPLEMENT_OBJECT_WRAPPER_ENCODER(WeakObjectPtr)
	IMPLEMENT_OBJECT_WRAPPER_ENCODER(LazyObjectPtr)
	IMPLEMENT_OBJECT_WRAPPER_ENCODER(ScriptInterface)
	IMPLEMENT_OBJECT_WRAPPER_ENCODER(StrongObjectPtr)

#undef IMPLEMENT_OBJECT_WRAPPER_ENCODER

	IMPLEMENT_SELF_DESCRIPTIVE_ENCODER(ScriptArray, Array)

	IMPLEMENT_SELF_DESCRIPTIVE_ENCODER(ScriptDelegate, Delegate)
	IMPLEMENT_SELF_DESCRIPTIVE_ENCODER(MulticastScriptDelegate, MulticastDelegate)

#undef IMPLEMENT_SELF_DESCRIPTIVE_ENCODER
	
	template <typename... TArgs>
	struct TZTryDecodeZCallBufferSlot
	{
		
		static void EncodeStatic(const TTuple<TArgs...>& args, FZCallBuffer& buf)
		{
			Encode(args, buf, 0);
		}

		static void EncodeInstance(const TTuple<TArgs...>& args, FZCallBuffer& buf)
		{
			Encode(args, buf, 1);
		}

	private:
		static void Encode(const TTuple<TArgs...>& args, FZCallBuffer& buf, int32 indexOffset)
		{
			if constexpr (sizeof...(TArgs) > 0)
			{
				EncodeAt<0>(args, buf, indexOffset);
			}
		}

		template <int32 Index>
		static void EncodeAt(const TTuple<TArgs...>& args, FZCallBuffer& buf, int32 indexOffset)
		{
			using TArg = decltype(args.template Get<Index>());

			if constexpr (std::is_lvalue_reference_v<TArg> && !std::is_const_v<TArg>)
			{
				TZCallBufferSlotEncoder<std::decay_t<TArg>>::template Encode(args.template Get<Index>(), buf[Index + indexOffset]);
			}
			
			if constexpr (Index < sizeof...(TArgs) - 1)
			{
				EncodeAt<Index + 1>(args, buf, indexOffset);
			}
		}
		
	};
}


