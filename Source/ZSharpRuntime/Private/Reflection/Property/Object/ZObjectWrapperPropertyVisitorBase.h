// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZObjectPropertyVisitorBase.h"
#include "ZCall/ZCallBufferSlotEncoder.h"

namespace ZSharp
{
	template <typename TProperty, typename TSelfDescriptiveObjectWrapper, typename TConjugateRegistry>
	class TZObjectWrapperPropertyVisitorBase : public FZObjectPropertyVisitorBase
	{

		using UnderlyingInstanceType = typename TSelfDescriptiveObjectWrapper::UnderlyingInstanceType;
		
	public:
		explicit TZObjectWrapperPropertyVisitorBase(const TProperty* underlyingProperty)
			: FZObjectPropertyVisitorBase(underlyingProperty)
			, UnderlyingWrapperProperty(underlyingProperty){}

	public:
		virtual void GetValue(const void* src, FZCallBufferSlot& dest) const override
		{
			// Special case for TObjectPtr<UClass> and UClass*.
			if constexpr (std::is_same_v<TProperty, FClassProperty>)
			{
				if (UnderlyingWrapperProperty->HasAllPropertyFlags(CPF_TObjectPtr) || !UnderlyingWrapperProperty->HasAnyPropertyFlags(CPF_UObjectWrapper))
				{
					TZCallBufferSlotEncoder<UClass*>::Encode(CastChecked<UClass>(UnderlyingWrapperProperty->GetPropertyValue(src), ECastCheckedType::NullAllowed), dest);
					return;
				}
			}
			
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
			// Special case for TObjectPtr<UClass> and UClass*.
			if constexpr (std::is_same_v<TProperty, FClassProperty>)
			{
				if (UnderlyingWrapperProperty->HasAllPropertyFlags(CPF_TObjectPtr) || !UnderlyingWrapperProperty->HasAnyPropertyFlags(CPF_UObjectWrapper))
				{
					TZCallBufferSlotEncoder<UClass*>::Encode(CastChecked<UClass>(UnderlyingWrapperProperty->GetPropertyValue(src), ECastCheckedType::NullAllowed), dest);
					return;
				}
			}
			
			dest.WriteConjugate(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<TConjugateRegistry>().Conjugate(GetDescriptor(), (UnderlyingInstanceType*)src));
		}
		
		virtual void SetValue(void* dest, const FZCallBufferSlot& src) const override
		{
			// Special case for TObjectPtr<UClass> and UClass*.
			if constexpr (std::is_same_v<TProperty, FClassProperty>)
			{
				if (UnderlyingWrapperProperty->HasAllPropertyFlags(CPF_TObjectPtr) || !UnderlyingWrapperProperty->HasAnyPropertyFlags(CPF_UObjectWrapper))
				{
					UClass* value = TZCallBufferSlotEncoder<UClass*>::Decode(src);
					check(!value || value->IsChildOf(UnderlyingWrapperProperty->MetaClass));
					UnderlyingProperty->CopySingleValue(dest, &value);
					return;
				}
			}
			
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
			if constexpr (std::is_same_v<TProperty, FInterfaceProperty>)
			{
				return UnderlyingWrapperProperty->InterfaceClass;
			}
			else if constexpr (std::is_same_v<TProperty, FClassProperty> || std::is_same_v<TProperty, FSoftClassProperty>)
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
			check(value.GetDescriptor()->IsChildOf(GetDescriptor()));
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


