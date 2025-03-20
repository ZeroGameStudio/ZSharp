// Copyright Zero Games. All Rights Reserved.

namespace System.Runtime.CompilerServices;

/// <summary>
/// The C# compiler used to have a CallerColumnNumber service,
/// but it's no longer found, may have been removed? (Thanks, Microsoft!)
/// This makes no sense but improve readability.
/// </summary>
[AttributeUsage(AttributeTargets.Parameter)]
public sealed class CallerColumnNumberAttribute : Attribute;


