// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	template <typename T>
	struct TZSelfDescriptiveObjectWrapperBase
	{

		static constexpr bool IsClass = std::is_same_v<T, TSubclassOf<UObject>> || std::is_same_v<T, TSoftClassPtr<UObject>>;
		using WrapperType = T;
		using ObjectType = std::conditional_t<IsClass, UClass, UObject>;

		TZSelfDescriptiveObjectWrapperBase(const UClass* descriptor)
			: Descriptor(descriptor)
			, UnderlyingInstance(new T)
			, bOwning(true){}
	
		TZSelfDescriptiveObjectWrapperBase(const UClass* descriptor, T* underlyingInstance)
			: Descriptor(descriptor)
			, UnderlyingInstance(underlyingInstance)
			, bOwning(false){}

		TZSelfDescriptiveObjectWrapperBase(const TZSelfDescriptiveObjectWrapperBase&) = delete;
		TZSelfDescriptiveObjectWrapperBase(TZSelfDescriptiveObjectWrapperBase&& other) noexcept { *this = MoveTemp(other); }

		~TZSelfDescriptiveObjectWrapperBase()
		{
			if (bOwning)
			{
				delete UnderlyingInstance;
			}
		}

		const UClass* GetDescriptor() const { return Descriptor.Get(); }
		T* GetUnderlyingInstance() const { return UnderlyingInstance; }
		UObject* Get() const
		{
			if constexpr (std::is_same_v<WrapperType, FScriptInterface>)
			{
				return UnderlyingInstance->GetObject();
			}
			else
			{
				return UnderlyingInstance->Get();
			}
		}
		void Set(ObjectType* obj)
		{
			if constexpr (std::is_same_v<WrapperType, FScriptInterface>)
			{
				*UnderlyingInstance = WrapperType { obj, obj->GetInterfaceAddress(const_cast<UClass*>(Descriptor.Get())) };
			}
			else
			{
				*UnderlyingInstance = WrapperType { obj };
			}
		}

		TZSelfDescriptiveObjectWrapperBase& operator=(const TZSelfDescriptiveObjectWrapperBase&) = delete;
		TZSelfDescriptiveObjectWrapperBase& operator=(TZSelfDescriptiveObjectWrapperBase&& other) noexcept
		{
			Descriptor = other.Descriptor;
			UnderlyingInstance = other.UnderlyingInstance;
			bOwning = other.bOwning;
			other.bOwning = false;
			
			return *this;
		}
	
	private:
		TStrongObjectPtr<const UClass> Descriptor;
		T* UnderlyingInstance;
		bool bOwning;
		
	};
}

#define DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER(Name, Wrapper) \
struct FZSelfDescriptive##Name : TZSelfDescriptiveObjectWrapperBase<Wrapper> \
{ \
	FZSelfDescriptive##Name(const UClass* descriptor) : TZSelfDescriptiveObjectWrapperBase(descriptor){} \
	FZSelfDescriptive##Name(const UClass* descriptor, WrapperType* underlyingInstance) : TZSelfDescriptiveObjectWrapperBase(descriptor, underlyingInstance){} \
	FZSelfDescriptive##Name(FZSelfDescriptive##Name&& other) noexcept : TZSelfDescriptiveObjectWrapperBase(MoveTemp(other)){} \
	static FString GetExportTypeName() { return #Name; } \
};

namespace ZSharp
{
	DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER(SubclassOf, TSubclassOf<UObject>)
	DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER(SoftClassPtr, TSoftClassPtr<UObject>)
	DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER(SoftObjectPtr, FSoftObjectPtr)
	DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER(WeakObjectPtr, FWeakObjectPtr)
	DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER(LazyObjectPtr, FLazyObjectPtr)
	DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER(ScriptInterface, FScriptInterface)
	DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER(StrongObjectPtr, TStrongObjectPtr<UObject>)
}

#undef DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER


