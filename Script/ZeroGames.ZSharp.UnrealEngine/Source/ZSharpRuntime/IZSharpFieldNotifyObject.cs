// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.UnrealEngine.FieldNotification;

namespace ZeroGames.ZSharp.UnrealEngine.ZSharpRuntime;

partial interface IZSharpFieldNotifyObject
{
    void BroadcastFieldValueChanged(FFieldNotificationId fieldId) => UFieldNotificationLibrary.BroadcastFieldValueChanged((UObject)this, fieldId);
}

public static class ZSharpFieldNotifyObjectExtensions
{
    extension(IZSharpFieldNotifyObject @this)
    {
        void BroadcastFieldValueChanged(FFieldNotificationId fieldId) => @this.BroadcastFieldValueChanged(fieldId);
        void BroadcastFieldValueChanged(FName fieldName) => @this.BroadcastFieldValueChanged(new FFieldNotificationId { FieldName = fieldName });
        void BroadcastFieldValueChanged(string fieldName) => @this.BroadcastFieldValueChanged(new FFieldNotificationId { FieldName = fieldName });
    }
}


