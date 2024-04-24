// Copyright Zero Games. All Rights Reserved.

#pragma once

#define ZCALL_TO_MANAGED(Name) int32 Name(TFunctionRef<void(struct FZCallBuffer*)> initialize, int32 numSlots) const
#define ZCALL_TO_MANAGED_NO_BUFFER(Name) int32 Name() const
#define ZCALL_TO_MANAGED_AUTO_BUFFER(Name) int32 Name(TFunctionRef<void(struct FZCallBuffer&, int32)> initialize) const
#define ZCALL_TO_MANAGED_USER_BUFFER(Name) int32 Name(TFunctionRef<struct FZCallBuffer*()> buffer) const
#define ZCALL_TO_MANAGED_USER_BUFFER_CHECKED(Name) int32 Name(TFunctionRef<struct FZCallBuffer*()> buffer, int32 numSlots) const


