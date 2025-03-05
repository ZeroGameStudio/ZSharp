// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	template <typename T>
	struct TZSelfDescriptiveObjectWrapperBase
	{

		static constexpr bool IsClass = std::is_same_v<T, TSubclassOf<UObject>> || std::is_same_v<T, TSoftClassPtr<UObject>>;
		using UnderlyingInstanceType = T;
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
		ObjectType* Get(bool evenIfGarbage = false) const
		{
			ObjectType* result;
			if constexpr (std::is_same_v<UnderlyingInstanceType, FScriptInterface>)
			{
				result = UnderlyingInstance->GetObject();
			}
			else
			{
				result = UnderlyingInstance->Get();
			}

			if (!result)
			{
				return nullptr;
			}

			if (result->HasAllFlags(RF_MirroredGarbage) && !evenIfGarbage)
			{
				return nullptr;
			}

			return result;
		}
		void Set(ObjectType* obj)
		{
			if constexpr (IsClass)
			{
				if (!ensure(obj->IsChildOf(GetDescriptor())))
				{
					return;
				}
			}
			else if constexpr (std::is_same_v<UnderlyingInstanceType, FScriptInterface>)
			{
				if (!ensure(obj->GetClass()->ImplementsInterface(GetDescriptor())))
				{
					return;
				}
			}
			else
			{
				if (!ensure(obj->IsA(GetDescriptor())))
				{
					return;
				}
			}
			
			if constexpr (std::is_same_v<UnderlyingInstanceType, FScriptInterface>)
			{
				*UnderlyingInstance = UnderlyingInstanceType { obj, obj->GetInterfaceAddress(const_cast<UClass*>(Descriptor.Get())) };
			}
			else
			{
				*UnderlyingInstance = UnderlyingInstanceType { obj };
			}
		}
		bool IsValid(bool evenIfGarbage = false) const
		{
			UObject* result;
			if constexpr (std::is_same_v<UnderlyingInstanceType, FScriptInterface>)
			{
				result = UnderlyingInstance->GetObject();
			}
			else
			{
				result = UnderlyingInstance->Get();
			}

			if (!result)
			{
				return false;
			}

			if (result->HasAllFlags(RF_MirroredGarbage) && !evenIfGarbage)
			{
				return false;
			}

			return true;
		}
		bool IsNull() const
		{
			if constexpr (std::is_same_v<UnderlyingInstanceType, FWeakObjectPtr>)
			{
				return UnderlyingInstance->IsExplicitlyNull();
			}
			else if constexpr (std::is_same_v<UnderlyingInstanceType, TSoftClassPtr<UObject>> ||
							   std::is_same_v<UnderlyingInstanceType, FSoftObjectPtr> ||
							   std::is_same_v<UnderlyingInstanceType, FLazyObjectPtr>)
			{
				return UnderlyingInstance->IsNull();
			}
			else
			{
				return !Get(true);
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
	
	protected:
		TStrongObjectPtr<const UClass> Descriptor;
		T* UnderlyingInstance;
		bool bOwning;
		
	};

#define BEGIN_DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER(Name, Wrapper) \
struct FZSelfDescriptive##Name : TZSelfDescriptiveObjectWrapperBase<Wrapper> \
{ \
	FZSelfDescriptive##Name(const UClass* descriptor) : TZSelfDescriptiveObjectWrapperBase(descriptor){} \
	FZSelfDescriptive##Name(const UClass* descriptor, UnderlyingInstanceType* underlyingInstance) : TZSelfDescriptiveObjectWrapperBase(descriptor, underlyingInstance){} \
	FZSelfDescriptive##Name(FZSelfDescriptive##Name&& other) noexcept : TZSelfDescriptiveObjectWrapperBase(MoveTemp(other)){}

#define END_DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER() };

#define DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER(Name, Wrapper) \
BEGIN_DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER(Name, Wrapper) \
END_DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER()

#define DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER_WITH_ARO(Name, Wrapper, GetReferencedObject) \
BEGIN_DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER(Name, Wrapper) \
	void AddReferencedObjects(FReferenceCollector& collector) { collector.AddReferencedObject(UnderlyingInstance->GetReferencedObject()); } \
END_DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER()

DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER_WITH_ARO(SubclassOf, TSubclassOf<UObject>, GetGCPtr)
DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER(SoftClassPtr, TSoftClassPtr<UObject>)
DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER(SoftObjectPtr, FSoftObjectPtr)
DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER(WeakObjectPtr, FWeakObjectPtr)
DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER(LazyObjectPtr, FLazyObjectPtr)
DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER_WITH_ARO(ScriptInterface, FScriptInterface, GetObjectRef)

#undef DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER_WITH_ARO
#undef DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER
#undef END_DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER
#undef BEGIN_DECLARE_SELF_DESCRIPTIVE_OBJECT_WRAPPER
}


