// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedEnum.h"
#include "Concept/ZStaticallyExportableEnum.h"
#include "Trait/ZExportedTypeName.h"
#include "Trait/ZExportedTypeZCallBufferSlotType.h"
#include "Trait/ZManagedTypeInfo.h"

namespace ZSharp
{
	namespace ZStaticallyExportedEnum_Private
	{
		ZSHARPEXPORT_API bool RegisterEnum(IZExportedEnum* enm);
	}
	
	template <CZStaticallyExportableEnum T>
	class TZStaticallyExportedEnum : public IZExportedEnum
	{

		using UnderlyingType = __underlying_type(T);

	public:
		virtual FString GetName() const override { return TZExportedTypeName<T>::Get().Name; }
		virtual FString GetModule() const override { return TZManagedTypeInfo<T>::GetModuleName(); }
		virtual FString GetUnrealFieldPath() const override { return {}; }
		virtual EZCallBufferSlotType GetSlotType() const override { return TZExportedTypeZCallBufferSlotType_V<UnderlyingType>; }
		virtual EZExportedEnumFlags GetFlags() const override { return Flags; }
		virtual FString GetUnderlyingType() const override { return TZExportedTypeName<UnderlyingType>::Get().Name; }
		virtual void ForeachEnumValue(TFunctionRef<void(const FString&, const FString&)> action) const override
		{
			for (const auto& pair : Values)
			{
				action(pair.Key, pair.Value);
			}
		}

	protected:
		explicit TZStaticallyExportedEnum(bool flag)
			: Flags(flag ? EZExportedEnumFlags::Flags : EZExportedEnumFlags::None){}

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
				return FString::Printf(TEXT("%lld"), static_cast<int64>(value));
			}
		}

	private:
		EZExportedEnumFlags Flags;
		TArray<TPair<FString, FString>> Values;
		
	};
}


