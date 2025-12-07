// Copyright Zero Games. All Rights Reserved.

#pragma once

static_assert(sizeof(FVector) == 24);
static_assert(offsetof(FVector, X) == 0);
static_assert(offsetof(FVector, Y) == 8);
static_assert(offsetof(FVector, Z) == 16);

static_assert(sizeof(FRotator) == 24);
static_assert(offsetof(FRotator, Pitch) == 0);
static_assert(offsetof(FRotator, Yaw) == 8);
static_assert(offsetof(FRotator, Roll) == 16);

static_assert(sizeof(FQuat) == 32);
static_assert(offsetof(FQuat, X) == 0);
static_assert(offsetof(FQuat, Y) == 8);
static_assert(offsetof(FQuat, Z) == 16);
static_assert(offsetof(FQuat, W) == 24);

namespace ZSharp::ZInteropAssersions
{
#define ZSHARP_STEAL_MEMBER_VARIABLE(Type, MemberType, MemberName, Variable) \
static MemberType(Type::*Variable); \
template <decltype(Variable) MemberPtr> \
struct TMemberPtrInitializer_##Variable \
{ \
	TMemberPtrInitializer_##Variable() \
	{ \
		Variable = MemberPtr; \
	} \
	static TMemberPtrInitializer_##Variable GInstance; \
}; \
template <decltype(Variable) MemberPtr> TMemberPtrInitializer_##Variable<MemberPtr> TMemberPtrInitializer_##Variable<MemberPtr>::GInstance; \
template struct TMemberPtrInitializer_##Variable<&Type::MemberName>;
	
#if ENABLE_VECTORIZED_TRANSFORM
	ZSHARP_STEAL_MEMBER_VARIABLE(FTransform, VectorRegister4Double, Rotation, GTransformRotationMemberPtr);
	ZSHARP_STEAL_MEMBER_VARIABLE(FTransform, VectorRegister4Double, Translation, GTransformTranslationMemberPtr);
	ZSHARP_STEAL_MEMBER_VARIABLE(FTransform, VectorRegister4Double, Scale3D, GTransformScale3DMemberPtr);
#else
	ZSHARP_STEAL_MEMBER_VARIABLE(FTransform, FQuat, Rotation, GTransformRotationMemberPtr);
	ZSHARP_STEAL_MEMBER_VARIABLE(FTransform, FVector, Translation, GTransformTranslationMemberPtr);
	ZSHARP_STEAL_MEMBER_VARIABLE(FTransform, FVector, Scale3D, GTransformScale3DMemberPtr);
#endif
	
#undef ZSHARP_STEAL_MEMBER_VARIABLE
	
	inline struct FZRuntimeAssertions
	{
		FZRuntimeAssertions()
		{
			// FTransform transform{};
			// auto p = reinterpret_cast<uint8*>(&transform);
			// auto r = reinterpret_cast<uint8*>(&(transform.*GTransformRotationMemberPtr));
			// auto t = reinterpret_cast<uint8*>(&(transform.*GTransformTranslationMemberPtr));
			// auto s = reinterpret_cast<uint8*>(&(transform.*GTransformScale3DMemberPtr));
			// check(r - p == 0);
			// check(t - p == 32);
			// check(s - p == 64);
		}
	} GRuntimeAssertions;
}


 