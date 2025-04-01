// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Misc/Log/ZRegisterLogCategory.h"

#define ZSHARP_REGISTER_LOG_CATEGORY_COMBINE_INNER(A, B, C) A##B##C
#define ZSHARP_REGISTER_LOG_CATEGORY_COMBINE(A, B, C) ZSHARP_REGISTER_LOG_CATEGORY_COMBINE_INNER(A, B, C)

/**
 * Registers a log category to managed side.
 * 
 * @param Category a LogCategoryBase& to register.
 * @param CategoryName An optional name only used to build variable name.
 */
#if NO_LOGGING
#define ZSHARP_REGISTER_LOG_CATEGORY_EX(Category, CategoryName)
#else
#define ZSHARP_REGISTER_LOG_CATEGORY_EX(Category, CategoryName) namespace ZSharp::__ZRegisterLogCategory_Private { static FZRegisterLogCategory ZSHARP_REGISTER_LOG_CATEGORY_COMBINE(__GRegisterLogCategory, CategoryName, __LINE__) { Category }; }
#endif

#define ZSHARP_REGISTER_LOG_CATEGORY(Category) ZSHARP_REGISTER_LOG_CATEGORY_EX(Category, Category)


