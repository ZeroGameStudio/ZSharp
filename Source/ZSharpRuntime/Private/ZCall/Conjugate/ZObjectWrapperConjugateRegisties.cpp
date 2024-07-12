﻿// Copyright Zero Games. All Rights Reserved.


#include "ZCall/Conjugate/ZObjectWrapperConjugateRegisties.h"

#include "Conjugate/ZDeclareConjugateRegistry.h"

namespace ZSharp::ZConjugateRegistry_UObject_Private
{
	static TZDeclareConjugateRegistry<FZConjugateRegistry_SubclassOf> GDeclareSubclassOf;
	static TZDeclareConjugateRegistry<FZConjugateRegistry_SoftClassPtr> GDeclareSoftClassPtr;
	static TZDeclareConjugateRegistry<FZConjugateRegistry_SoftObjectPtr> GDeclareSoftObjectPtr;
	static TZDeclareConjugateRegistry<FZConjugateRegistry_WeakObjectPtr> GDeclareWeakObjectPtr;
	static TZDeclareConjugateRegistry<FZConjugateRegistry_LazyObjectPtr> GDeclareLazyObjectPtr;
	static TZDeclareConjugateRegistry<FZConjugateRegistry_ScriptInterface> GDeclareScriptInterface;
	static TZDeclareConjugateRegistry<FZConjugateRegistry_StrongObjectPtr> GDeclareStrongObjectPtr;
}
