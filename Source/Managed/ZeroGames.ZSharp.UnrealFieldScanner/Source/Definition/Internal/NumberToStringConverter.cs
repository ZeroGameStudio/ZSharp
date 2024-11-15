// Copyright Zero Games. All Rights Reserved.

using System.Text.Json;
using System.Text.Json.Serialization;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class NumberToStringConverter : JsonConverter<object>
{
	
	public override bool CanConvert(Type typeToConvert)
	{
		return typeToConvert == typeof(uint8) ||
		       typeToConvert == typeof(uint16) ||
		       typeToConvert == typeof(uint32) ||
		       typeToConvert == typeof(uint64) ||
		       typeToConvert == typeof(int8) ||
		       typeToConvert == typeof(int16) ||
		       typeToConvert == typeof(int32) ||
		       typeToConvert == typeof(int64) ||
		       typeToConvert == typeof(float) ||
		       typeToConvert == typeof(double) ||
		       typeToConvert.IsEnum;
	}

	public override object Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options) => throw new InvalidOperationException();

	public override void Write(Utf8JsonWriter writer, object value, JsonSerializerOptions options)
	{
		object number = value is Enum ? Convert.ChangeType(value, value.GetType().GetEnumUnderlyingType()) : value;
		writer.WriteStringValue(number.ToString());
	}
	
}


