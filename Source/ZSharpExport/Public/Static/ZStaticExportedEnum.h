// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedEnum.h"
#include "Concept/ZStaticExportableEnum.h"
#include "Trait/ZExportedTypeModule.h"
#include "Trait/ZExportedTypeName.h"
#include "Trait/ZExportedTypeZCallBufferSlotType.h"
#include "IZExportedTypeRegistry.h"

namespace ZSharp
{
	namespace ZStaticExportedEnum_Private
	{
		ZSHARPEXPORT_API bool RegisterEnum(IZExportedEnum* enm);
	}
	
	template <CZStaticExportableEnum T>
	class TZStaticExportedEnum : public IZExportedEnum
	{

		friend struct FZFinalizer;

		using UnderlyingType = __underlying_type(T);

	public:
		virtual bool IsRegistered() const override { return bRegistered; }
		virtual FString GetName() const override { return TZExportedTypeName<T>::Get(); }
		virtual FString GetModule() const override { return TZExportedTypeModule<T>::Get(); }
		virtual EZCallBufferSlotType GetSlotType() const override { return TZExportedTypeZCallBufferSlotType<UnderlyingType>::Get(); }
		virtual FString GetUnderlyingType() const override { return TZExportedTypeName<UnderlyingType>::Get(); }
		virtual void ForeachEnumValue(TFunctionRef<void(const FString&, const FString&)> action) const override
		{
			for (const auto& pair : Values)
			{
				action(pair.Key, pair.Value);
			}
		}

	protected:
		struct FZFinalizer
		{
			TZStaticExportedEnum* Enum;
			~FZFinalizer()
			{
				 Enum->bRegistered = ZStaticExportedEnum_Private::RegisterEnum(Enum);
			}
		};

	protected:
		explicit TZStaticExportedEnum(const FZFinalizer&)
			: bRegistered(false){}

	protected:
		void AddEnumValue(const FString& name, T value) { Values.Emplace(TPair<FString, FString> { name, ValueToString(value) }); }

	private:
		static FString ValueToString(T value)
		{
			if constexpr (std::is_same_v<UnderlyingType, uint64>)
			{
				return FString::Printf(TEXT("%llu"), value);
			}
			else
			{
				return FString::Printf(TEXT("%lld"), StaticCast<int64>(value));
			}
		}

	private:
		bool bRegistered;
		TArray<TPair<FString, FString>> Values;
		
	};
}

#define ZSHARP_DECLARE_EXPORTED_ENUM(Enum, Name, Module) \
ZSHARP_EXPORT_TYPE_NAME_EX(Enum, Name) \
ZSHARP_EXPORT_TYPE_MODULE(Enum, Module)

#define ZSHARP_BEGIN_EXPORT_ENUM(Enum) \
namespace __ZSharpExport_Private \
{ \
	static struct __FZStaticExportedEnum_##Enum : public ZSharp::TZStaticExportedEnum<Enum> \
	{ \
		using EnumClass = Enum; \
		using ThisClass = __FZStaticExportedEnum_##Enum; \
		explicit __FZStaticExportedEnum_##Enum(const FZFinalizer& finalizer) : ZSharp::TZStaticExportedEnum<Enum>(finalizer) \
		{

#define ZSHARP_EXPORT_ENUM_VALUE(Value) AddEnumValue(#Value, EnumClass::Value);

#define ZSHARP_END_EXPORT_ENUM(Enum) \
			static_assert(std::is_same_v<ThisClass, __FZStaticExportedEnum_##Enum>, "Enum name doesn't match between BEGIN_EXPORT and END_EXPORT!"); \
		} \
	} __GExportedEnum_##Enum { { &__GExportedEnum_##Enum } }; \
}


