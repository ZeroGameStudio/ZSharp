// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZSelfDescriptiveTraitsBase
	{
		static constexpr bool HasCustomNewUnderlyingInstance = false;
		static constexpr bool HasCustomDeleteUnderlyingInstance = false;
		static constexpr bool HasCustomDeleteDescriptor = false;
		static constexpr bool HasFinalize = false;
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
		TZSelfDescriptiveBase(const TDescriptor* descriptor) : TZSelfDescriptiveBase(descriptor, false){}
		TZSelfDescriptiveBase(const TDescriptor* descriptor, bool ownsDescriptor)
			: Descriptor(descriptor)
			, UnderlyingInstance(NewUnderlyingInstance(descriptor))
			, bOwning(true)
			, bOwnsDescriptor(ownsDescriptor)
		{
			if constexpr (IsUObjectDescriptor)
			{
				check(!ownsDescriptor);
				GCRoot = TStrongObjectPtr<const UObject> { Descriptor };
			}
		}
		
		TZSelfDescriptiveBase(const TDescriptor* descriptor, TUnderlyingInstance* underlyingInstance)
			: Descriptor(descriptor)
			, UnderlyingInstance(underlyingInstance)
			, bOwning(false)
			, bOwnsDescriptor(false)
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
			bOwnsDescriptor = other.bOwnsDescriptor;
			other.bOwnsDescriptor = false;
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
			}

			if (bOwnsDescriptor)
			{
				if constexpr (TZSelfDescriptiveTraits<TImpl>::HasCustomDeleteDescriptor)
				{
					AsImpl().DeleteDescriptor();
				}
				else if (!IsUObjectDescriptor)
				{
					delete Descriptor;
				}
				else
				{
					checkNoEntry();
				}
			}

			if constexpr (TZSelfDescriptiveTraits<TImpl>::HasFinalize)
			{
				AsImpl().Finalize();
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
			bOwnsDescriptor = other.bOwnsDescriptor;
			other.bOwnsDescriptor = false;

			return *this;
		}

	private:
		static TUnderlyingInstance* NewUnderlyingInstance(const TDescriptor* descriptor)
		{
			if constexpr (TZSelfDescriptiveTraits<TImpl>::HasCustomNewUnderlyingInstance)
			{
				return TImpl::NewUnderlyingInstance(descriptor);
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
		bool bOwnsDescriptor;
		
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

#define ZSHARP_SELF_DESCRIPTIVE_GENERATED_BODY_WITH_OWNS_DESCRIPTOR(ThisClass) \
	using Super = TZSelfDescriptiveBase; \
	friend Super; \
	ThisClass(const DescriptorType* descriptor, bool ownsDescriptor); \
	ThisClass(const DescriptorType* descriptor, UnderlyingInstanceType* underlyingInstance);


