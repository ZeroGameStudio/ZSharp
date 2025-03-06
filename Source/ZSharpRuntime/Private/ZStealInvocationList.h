// Copyright Zero Games. All Rights Reserved.

#pragma once

/**
 * This magic helps us to access multicast delegate invocation list.
 * 
 * Usage:
 * Define a static global variable: ZSHARP_STEAL_INVOCATION_LIST(GInvocationListMemberPtr)
 * Get the invocation list of a delegate: auto& invocationList = delegate.*GInvocationListMemberPtr;
 */
#define ZSHARP_STEAL_INVOCATION_LIST(Variable) \
static TArray<TDelegateBase<FNotThreadSafeNotCheckedDelegateMode>, FMulticastInvocationListAllocatorType>(TMulticastDelegateBase<FDefaultDelegateUserPolicy>::*Variable); \
template <decltype(Variable) MemberPtr> \
struct TInvocationListMemberPtrInitializer \
{ \
	TInvocationListMemberPtrInitializer() \
	{ \
		Variable = MemberPtr; \
	} \
	static TInvocationListMemberPtrInitializer GInstance; \
}; \
template <decltype(Variable) MemberPtr> TInvocationListMemberPtrInitializer<MemberPtr> TInvocationListMemberPtrInitializer<MemberPtr>::GInstance; \
template struct TInvocationListMemberPtrInitializer<&TMulticastDelegateBase<FDefaultDelegateUserPolicy>::InvocationList>;


