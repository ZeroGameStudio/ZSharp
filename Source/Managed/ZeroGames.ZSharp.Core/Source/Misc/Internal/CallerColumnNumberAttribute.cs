// Copyright Zero Games. All Rights Reserved.

namespace System.Runtime.CompilerServices;

/// <summary>
/// The C# compiler used to have a CallerColumnNumber service,
/// but it's no longer found, may have been removed? (Thanks, Microsoft!)
/// </summary>
[AttributeUsage(AttributeTargets.Parameter)]
internal sealed class CallerColumnNumberAttribute : Attribute;


