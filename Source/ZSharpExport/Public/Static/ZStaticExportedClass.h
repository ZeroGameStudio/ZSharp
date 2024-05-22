// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedClass.h"
#include "Concept/ZStaticExportableClass.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Trait/ZExportedTypeModule.h"
#include "Trait/ZExportedTypeName.h"

namespace ZSharp
{
	namespace ZStaticExportedClass_Private
	{
		ZSHARPEXPORT_API bool RegisterClass(IZExportedClass* cls);
	}
	
	template <CZStaticExportableClass T>
	class TZStaticExportedClass : public IZExportedClass
	{

		friend struct FZFinalizer;

	public:
		virtual bool IsRegistered() const override { return bRegistered; }
		virtual FString GetName() const override { return TZExportedTypeName<T>::Get(); }
		virtual FString GetModule() const override { return TZExportedTypeModule<T>::Get(); }
		virtual FString GetUnrealFieldPath() const override { return {}; }
		virtual FString GetOuterExportName() const override { return GetInnerExportName(); }
		virtual uint16 GetConjugateRegistryId() const override { return TZConjugateRegistryId_V<T>; }
		virtual EZExportedClassFlags GetFlags() const override { return Flags; }
		virtual FString GetBaseType() const override { return {}; }

	protected:
		struct FZFinalizer
		{
			TZStaticExportedClass* Class;
			~FZFinalizer()
			{
				Class->bRegistered = ZStaticExportedClass_Private::RegisterClass(Class);
			}
		};

	protected:
		TZStaticExportedClass(bool bAbstract, const FZFinalizer&)
			: bRegistered(false)
			, Flags(EZExportedClassFlags::Plain | (bAbstract ? EZExportedClassFlags::Abstract : EZExportedClassFlags::None)){}

	private:
		bool bRegistered;
		EZExportedClassFlags Flags;
		
	};
}

#define ZSHARP_DECLARE_EXPORTED_CLASS(Class, Name, Module) \
ZSHARP_EXPORT_TYPE_NAME_EX(Class, Name) \
ZSHARP_EXPORT_TYPE_MODULE(Class, Module)

#define ZSHARP_BEGIN_EXPORT_CLASS(Class) \
namespace __ZSharpExport_Private \
{ \
	static struct __FZStaticExportedClass_##Class : public ZSharp::TZStaticExportedClass<Class> \
	{ \
		using ClassClass = Class; \
		using ThisClass = __FZStaticExportedClass_##Class; \
		explicit __FZStaticExportedClass_##Class(const FZFinalizer& finalizer) : ZSharp::TZStaticExportedClass<Class>(false, finalizer) \
		{

#define ZSHARP_END_EXPORT_Class(Class) \
			static_assert(std::is_same_v<ThisClass, __FZStaticExportedClass_##Class>, "Class name doesn't match between BEGIN_EXPORT and END_EXPORT!"); \
		} \
	} __GExportedClass_##Class { { &__GExportedClass_##Class } }; \
}


