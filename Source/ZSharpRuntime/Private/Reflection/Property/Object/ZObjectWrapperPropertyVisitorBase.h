// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZObjectPropertyVisitorBase.h"
#include "ZCall/ZCallBufferSlotEncoder.h"

namespace ZSharp
{
	template <typename TProperty, typename TSelfDescriptiveObjectWrapper, typename TConjugateRegistry>
	class TZObjectWrapperPropertyVisitorBase : public FZObjectPropertyVisitorBase
	{

		using WrapperType = typename TSelfDescriptiveObjectWrapper::WrapperType;
		
	public:
		explicit TZObjectWrapperPropertyVisitorBase(const TProperty* underlyingProperty)
			: FZObjectPropertyVisitorBase(underlyingProperty)
			, UnderlyingWrapperProperty(underlyingProperty){}

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override
		{
			TSelfDescriptiveObjectWrapper* sdow = TZCallBufferSlotEncoder<TSelfDescriptiveObjectWrapper>::DecodePointer(dest);
			if (!sdow)
			{
				dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>().Conjugate(GetDescriptor(), [this, src](const TSelfDescriptiveObjectWrapper& sdow){ UnderlyingProperty->CopySingleValue(sdow.GetUnderlyingInstance(), src); }));
			}
			else
			{
				check(sdow->GetDescriptor() == GetDescriptor());
				UnderlyingProperty->CopySingleValue(sdow->GetUnderlyingInstance(), src);
			}
		}
		
		virtual void GetRef(const void* src, FZCallBufferSlot& dest) const override
		{
			dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>().Conjugate(GetDescriptor(), (WrapperType*)src));
		}
		
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override
		{
			const TSelfDescriptiveObjectWrapper* value = TZCallBufferSlotEncoder<TSelfDescriptiveObjectWrapper>::DecodePointer(src);
			if (!value)
			{
				UnderlyingProperty->InitializeValue(dest);
				return;
			}

			ValidateValue(*value);
			UnderlyingProperty->CopySingleValue(dest, value->GetUnderlyingInstance());
		}

	private:
		const UClass* GetDescriptor() const
		{
			if constexpr (std::is_same_v<WrapperType, FScriptInterface>)
			{
				return UnderlyingWrapperProperty->InterfaceClass;
			}
			else if constexpr (std::is_same_v<WrapperType, TSoftClassPtr<UObject>> || std::is_same_v<WrapperType, TSubclassOf<UObject>>)
			{
				return UnderlyingWrapperProperty->MetaClass;
			}
			else
			{
				return UnderlyingWrapperProperty->PropertyClass;
			}
		}

		void ValidateValue(const TSelfDescriptiveObjectWrapper& value) const
		{
			// @TODO
		}

	private:
		const TProperty* UnderlyingWrapperProperty;
		
	};
}

#define DECLARE_OBJECT_WRAPPER_PROPERTY_VISITOR(PropertyName, WrapperName) \
	class FZ##PropertyName##PropertyVisitor final : public TZObjectWrapperPropertyVisitorBase<F##PropertyName##Property, FZSelfDescriptive##WrapperName, FZConjugateRegistry_##WrapperName> \
	{ \
	public: \
		explicit FZ##PropertyName##PropertyVisitor(const F##PropertyName##Property* underlyingProperty) : TZObjectWrapperPropertyVisitorBase(underlyingProperty){} \
	};


