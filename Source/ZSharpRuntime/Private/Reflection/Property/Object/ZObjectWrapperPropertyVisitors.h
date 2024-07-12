// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZObjectWrapperPropertyVisitorBase.h"

namespace ZSharp
{
	DECLARE_OBJECT_WRAPPER_PROPERTY_VISITOR(Class, SubclassOf);
	DECLARE_OBJECT_WRAPPER_PROPERTY_VISITOR(SoftClass, SoftClassPtr);
	DECLARE_OBJECT_WRAPPER_PROPERTY_VISITOR(SoftObject, SoftObjectPtr);
	DECLARE_OBJECT_WRAPPER_PROPERTY_VISITOR(WeakObject, WeakObjectPtr);
	DECLARE_OBJECT_WRAPPER_PROPERTY_VISITOR(LazyObject, LazyObjectPtr);
	DECLARE_OBJECT_WRAPPER_PROPERTY_VISITOR(Interface, ScriptInterface);
}


