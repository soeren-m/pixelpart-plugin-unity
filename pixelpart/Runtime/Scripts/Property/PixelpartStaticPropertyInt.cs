using System;

namespace Pixelpart {
public class PixelpartStaticPropertyInt {
	public int Value => Plugin.PixelpartStaticPropertyIntValue(internalProperty);

	public int BaseValue {
		get => Plugin.PixelpartStaticPropertyIntGetBaseValue(internalProperty);
		set => Plugin.PixelpartStaticPropertyIntSetBaseValue(internalProperty, value);
	}

	private readonly IntPtr internalProperty;

	public PixelpartStaticPropertyInt(IntPtr internalPropertyPtr) {
		internalProperty = internalPropertyPtr;
	}

	[Obsolete("deprecated, use Value")]
	public int Get() => Value;
	[Obsolete("deprecated, use BaseValue")]
	public void SetValue(int value) => BaseValue = value;
	[Obsolete("deprecated, use BaseValue")]
	public int GetValue() => BaseValue;
}
}