// Copyright Zero Games. All Rights Reserved.

#include "ZSceneComponent_Interop.h"

#include "Conjugate/ZConjugateUnsafe.h"
#include "Interop/ZInteropExceptionHelper.h"

static_assert(std::is_same_v<std::underlying_type_t<ERelativeTransformSpace>, int32>);
static_assert(std::is_same_v<std::underlying_type_t<ETeleportType>, uint8>);

void ZSharp::FZSceneComponent_Interop::GetComponentTransform(FZConjugateHandle self, FTransform& transform)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        transform = pSelf->GetComponentTransform();
    );
}

void ZSharp::FZSceneComponent_Interop::GetComponentLocation(FZConjugateHandle self, FVector& location)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        location = pSelf->GetComponentLocation();
    );
}

void ZSharp::FZSceneComponent_Interop::GetComponentRotation(FZConjugateHandle self, FRotator& rotation)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        rotation = pSelf->GetComponentRotation();
    );
}

void ZSharp::FZSceneComponent_Interop::GetComponentScale(FZConjugateHandle self, FVector& scale)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        scale = pSelf->GetComponentScale();
    );
}

void ZSharp::FZSceneComponent_Interop::GetSocketTransform(FZConjugateHandle self, const TCHAR* socketName, ERelativeTransformSpace transformSpace, FTransform& transform)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        transform = pSelf->GetSocketTransform(socketName, transformSpace);
    );
}

void ZSharp::FZSceneComponent_Interop::GetSocketLocation(FZConjugateHandle self, const TCHAR* socketName, ERelativeTransformSpace transformSpace, FVector& location)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        location = pSelf->GetSocketTransform(socketName, transformSpace).GetTranslation();
    );
}

void ZSharp::FZSceneComponent_Interop::GetSocketRotation(FZConjugateHandle self, const TCHAR* socketName, ERelativeTransformSpace transformSpace, FRotator& rotation)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        rotation = pSelf->GetSocketTransform(socketName, transformSpace).GetRotation().Rotator();
    );
}

void ZSharp::FZSceneComponent_Interop::GetSocketScale(FZConjugateHandle self, const TCHAR* socketName, ERelativeTransformSpace transformSpace, FVector& scale)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        scale = pSelf->GetSocketTransform(socketName, transformSpace).GetScale3D();
    );
}

void ZSharp::FZSceneComponent_Interop::SetWorldTransform(FZConjugateHandle self, const FTransform& newTransform, uint8 sweep, ETeleportType teleport)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        FTransform trans = newTransform;
        pSelf->SetWorldTransform(trans, !!sweep, nullptr, teleport);
    );
}

void ZSharp::FZSceneComponent_Interop::SetWorldLocation(FZConjugateHandle self, const FVector& newLocation, uint8 sweep, ETeleportType teleport)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        pSelf->SetWorldLocation(newLocation, !!sweep, nullptr, teleport);
    );
}

void ZSharp::FZSceneComponent_Interop::SetWorldRotation(FZConjugateHandle self, const FRotator& newRotator, uint8 sweep, ETeleportType teleport)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        pSelf->SetWorldRotation(newRotator, !!sweep, nullptr, teleport);
    );
}

void ZSharp::FZSceneComponent_Interop::SetWorldLocationAndRotation(FZConjugateHandle self, const FVector& newLocation, const FRotator& newRotator, uint8 sweep, ETeleportType teleport)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        pSelf->SetWorldLocationAndRotation(newLocation, newRotator, !!sweep, nullptr, teleport);
    );
}

void ZSharp::FZSceneComponent_Interop::SetWorldScale(FZConjugateHandle self, const FVector& newScale)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        pSelf->SetWorldScale3D(newScale);
    );
}

void ZSharp::FZSceneComponent_Interop::SetRelativeTransform(FZConjugateHandle self, const FTransform& newTransform, uint8 sweep, ETeleportType teleport)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        FTransform trans = newTransform;
        pSelf->SetRelativeTransform(trans, !!sweep, nullptr, teleport);
    );
}

void ZSharp::FZSceneComponent_Interop::SetRelativeLocation(FZConjugateHandle self, const FVector& newLocation, uint8 sweep, ETeleportType teleport)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        pSelf->SetRelativeLocation(newLocation, !!sweep, nullptr, teleport);
    );
}

void ZSharp::FZSceneComponent_Interop::SetRelativeRotation(FZConjugateHandle self, const FRotator& newRotator, uint8 sweep, ETeleportType teleport)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        pSelf->SetRelativeRotation(newRotator, !!sweep, nullptr, teleport);
    );
}

void ZSharp::FZSceneComponent_Interop::SetRelativeScale(FZConjugateHandle self, const FVector& newScale)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        pSelf->SetRelativeScale3D(newScale);
    );
}

void ZSharp::FZSceneComponent_Interop::AddWorldTransform(FZConjugateHandle self, const FTransform& deltaTransform, uint8 sweep, ETeleportType teleport)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        FTransform trans = deltaTransform;
        pSelf->AddWorldTransform(trans, !!sweep, nullptr, teleport);
    );
}

void ZSharp::FZSceneComponent_Interop::AddWorldTransformKeepScale(FZConjugateHandle self, const FTransform& deltaTransform, uint8 sweep, ETeleportType teleport)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        FTransform trans = deltaTransform;
        pSelf->AddWorldTransformKeepScale(trans, !!sweep, nullptr, teleport);
    );
}

void ZSharp::FZSceneComponent_Interop::AddWorldOffset(FZConjugateHandle self, const FVector& deltaLocation, uint8 sweep, ETeleportType teleport)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        pSelf->AddWorldOffset(deltaLocation, !!sweep, nullptr, teleport);
    );
}

void ZSharp::FZSceneComponent_Interop::AddWorldRotation(FZConjugateHandle self, const FRotator& deltaRotator, uint8 sweep, ETeleportType teleport)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        pSelf->AddWorldRotation(deltaRotator, !!sweep, nullptr, teleport);
    );
}

void ZSharp::FZSceneComponent_Interop::AddLocalTransform(FZConjugateHandle self, const FTransform& deltaTransform, uint8 sweep, ETeleportType teleport)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        FTransform trans = deltaTransform;
        pSelf->AddLocalTransform(trans, !!sweep, nullptr, teleport);
    );
}

void ZSharp::FZSceneComponent_Interop::AddLocalOffset(FZConjugateHandle self, const FVector& deltaLocation, uint8 sweep, ETeleportType teleport)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        pSelf->AddLocalOffset(deltaLocation, !!sweep, nullptr, teleport);
    );
}

void ZSharp::FZSceneComponent_Interop::AddLocalRotation(FZConjugateHandle self, const FRotator& deltaRotator, uint8 sweep, ETeleportType teleport)
{
    GUARD
    (
        auto pSelf = ConjugateUnsafe<USceneComponent>(self);
        pSelf->AddLocalRotation(deltaRotator, !!sweep, nullptr, teleport);
    );
}