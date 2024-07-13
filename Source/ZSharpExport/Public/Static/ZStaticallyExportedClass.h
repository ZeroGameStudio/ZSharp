// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedClass.h"
#include "Concept/ZStaticallyExportableClass.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Trait/ZExportedTypeName.h"
#include "Trait/ZManagedTypeInfo.h"

namespace ZSharp
{
	namespace ZStaticallyExportedClass_Private
	{
		ZSHARPEXPORT_API bool RegisterClass(IZExportedClass* cls);
	}
	
	template <CZStaticallyExportableClass T>
	class TZStaticallyExportedClass : public IZExportedClass
	{

		friend struct FZFinalizer;

	public:
		virtual FString GetName() const override { return TZExportedTypeName<T>::Get().Name; }
		virtual FString GetModule() const override { return TZManagedTypeInfo<T>::GetModuleName(); }
		virtual FString GetUnrealFieldPath() const override { return {}; }
		virtual uint16 GetConjugateRegistryId() const override { return TZConjugateRegistryId_V<T>; }
		virtual EZExportedClassFlags GetFlags() const override { return Flags; }
		virtual FZFullyExportedTypeName GetBaseType() const override { return {}; }
		
		virtual void ForeachMethod(TFunctionRef<void(const IZExportedMethod&)> action) const override
		{
			
		}
		
		virtual void ForeachProperty(TFunctionRef<void(const IZExportedProperty&)> action) const override
		{
			
		}

	protected:
		TZStaticallyExportedClass(bool abstract)
			: Flags(EZExportedClassFlags::Plain | (abstract ? EZExportedClassFlags::Abstract : EZExportedClassFlags::None)){}

	private:
		EZExportedClassFlags Flags;
		
	};
}

#define ZSHARP_DECLARE_EXPORTED_CLASS(Class, Name, Module, RegistryId) \
ZSHARP_EXPORT_TYPE_NAME_EX(Class, Name) \
ZSHARP_EXPORT_TYPE_MODULE(Class, Module) \
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(Class, RegistryId)

#define ZSHARP_BEGIN_EXPORT_CLASS(Class) \
namespace __ZSharpExport_Private \
{ \
	static struct __FZStaticallyExportedClass_##Class : public ZSharp::TZStaticallyExportedClass<Class> \
	{ \
		using ClassClass = Class; \
		using ThisClass = __FZStaticallyExportedClass_##Class; \
		explicit __FZStaticallyExportedClass_##Class() : ZSharp::TZStaticallyExportedClass<Class>(false) \
		{

#define ZSHARP_END_EXPORT_CLASS(Class) \
			static_assert(std::is_same_v<ThisClass, __FZStaticallyExportedClass_##Class>, "Class name doesn't match between BEGIN_EXPORT and END_EXPORT!"); \
			ZSharp::ZStaticallyExportedClass_Private::RegisterClass(this); \
		} \
	}* __GExportedClass_##Class = new std::remove_pointer_t<decltype(__GExportedClass_##Class)>; \
}


