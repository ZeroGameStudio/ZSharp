// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.FieldNotification;

/// <summary>
/// All classes implement INotifyFieldValueChanged need to implement this pattern for source generator.
/// By default, we throw a runtime exception to pass compilation because some classes may not be used in managed code.
/// We can also benefit from this because source generator won't cast to INotifyFieldValueChanged to call this method
/// and will cause compilation error if the class doesn't give an implementation.
/// </summary>
partial interface INotifyFieldValueChanged
{
	void BroadcastFieldValueChanged(string fieldName) => Thrower.NotImplemented();
}


