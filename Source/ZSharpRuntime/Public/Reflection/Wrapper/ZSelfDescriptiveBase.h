// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZSelfDescriptiveTraitsBase
	{
		static constexpr bool HasCustomNewUnderlyingInstance = false;
		static constexpr bool HasCustomDeleteUnderlyingInstance = false;
		static constexpr bool HasCustomDeleteDescriptor = false;
	};

	template <typename>
	struct TZSelfDescriptiveTraits : FZSelfDescriptiveTraitsBase{};

	// IMPORTANT: Destructor is non-virtual and never use this type as pointer!
	template <typename TImpl, typename TDescriptor, typename TUnderlyingInstance>
	struct TZSelfDescriptiveBase
	{

		static constexpr bool IsUObjectDescriptor = TPointerIsConvertibleFromTo<TDescriptor, UObject>::Value;
		
		using ImplType = TImpl;
		using DescriptorType = TDescriptor;
		using UnderlyingInstanceType = TUnderlyingInstance;

	public:
		const TDescriptor* GetDescriptor() const { return Descriptor; }
		TUnderlyingInstance* GetUnderlyingInstance() const { return UnderlyingInstance; }

	protected:
		TZSelfDescriptiveBase(const TDescriptor* descriptor)
			: Descriptor(descriptor)
			, UnderlyingInstance(NewUnderlyingInstance())
			, bOwning(true)
		{
			if constexpr (IsUObjectDescriptor)
			{
				GCRoot = TStrongObjectPtr<const UObject> { Descriptor };
			}
		}
		
		TZSelfDescriptiveBase(const TDescriptor* descriptor, TUnderlyingInstance* underlyingInstance)
			: Descriptor(descriptor)
			, UnderlyingInstance(underlyingInstance)
			, bOwning(false)
		{
			if constexpr (IsUObjectDescriptor)
			{
				GCRoot = TStrongObjectPtr<const UObject> { Descriptor };
			}
		}
		
		TZSelfDescriptiveBase(const TZSelfDescriptiveBase&) = delete;
		TZSelfDescriptiveBase(TZSelfDescriptiveBase&& other) noexcept
		{
			Descriptor = other.Descriptor;
			UnderlyingInstance = other.UnderlyingInstance;
			bOwning = other.bOwning;
			other.bOwning = false;
		}
		
		~TZSelfDescriptiveBase()
		{
			if (bOwning)
			{
				if constexpr (TZSelfDescriptiveTraits<TImpl>::HasCustomDeleteUnderlyingInstance)
				{
					AsImpl().DeleteUnderlyingInstance();
				}
				else
				{
					delete UnderlyingInstance;
				}

				if constexpr (TZSelfDescriptiveTraits<TImpl>::HasCustomDeleteDescriptor)
				{
					AsImpl().DeleteDescriptor();
				}
				else if (!IsUObjectDescriptor)
				{
					delete Descriptor;
				}
			}
		}

		TZSelfDescriptiveBase& operator=(const TZSelfDescriptiveBase&) = delete;
		TZSelfDescriptiveBase& operator=(TZSelfDescriptiveBase&& other) noexcept
		{
			GCRoot = other.GCRoot;
			other.GCRoot.Reset();
			Descriptor = other.Descriptor;
			UnderlyingInstance = other.UnderlyingInstance;
			bOwning = other.bOwning;
			other.bOwning = false;

			return *this;
		}

	private:
		static TUnderlyingInstance* NewUnderlyingInstance()
		{
			if constexpr (TZSelfDescriptiveTraits<TImpl>::HasCustomNewUnderlyingInstance)
			{
				return TImpl::NewUnderlyingInstance();
			}
			else
			{
				return new TUnderlyingInstance;
			}
		}

	private:
		TImpl& AsImpl() { return *static_cast<TImpl*>(this); }
		const TImpl& AsImpl() const { return *static_cast<const TImpl*>(this); }

	protected:
		TStrongObjectPtr<const UObject> GCRoot;
		const TDescriptor* Descriptor;
		TUnderlyingInstance* UnderlyingInstance;
		bool bOwning;
		
	};
}

#define ZSHARP_SELF_DESCRIPTIVE_GENERATED_BODY(ThisClass) \
	using Super = TZSelfDescriptiveBase; \
	friend Super; \
	ThisClass(const DescriptorType* descriptor); \
	ThisClass(const DescriptorType* descriptor, UnderlyingInstanceType* underlyingInstance);

#define ZSHARP_SELF_DESCRIPTIVE_GENERATED_BODY_AUTO_CTOR(ThisClass) \
	using Super = TZSelfDescriptiveBase; \
	friend Super; \
	ThisClass(const DescriptorType* descriptor) : Super(descriptor){} \
	ThisClass(const DescriptorType* descriptor, UnderlyingInstanceType* underlyingInstance) : Super(descriptor, underlyingInstance){}


