using System;
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;

namespace Pixelpart {
[CustomPropertyDrawer(typeof(PixelpartVariantValue))]
public class EventOptionDrawer : PropertyDrawer {
	public override void OnGUI(Rect position, SerializedProperty property, GUIContent label) {
		EditorGUI.BeginProperty(position, label, property);

		PixelpartVariantValue.VariantType variantType = (PixelpartVariantValue.VariantType)(property.FindPropertyRelative("type").enumValueIndex - 1);
		SerializedProperty propertyX = property.FindPropertyRelative("x");
		SerializedProperty propertyY = property.FindPropertyRelative("y");
		SerializedProperty propertyZ = property.FindPropertyRelative("z");
		SerializedProperty propertyW = property.FindPropertyRelative("w");

		switch(variantType) {
			case PixelpartVariantValue.VariantType.Bool: {
				EditorGUI.BeginChangeCheck();
				bool modifiedValue = EditorGUI.Toggle(position, label, propertyX.floatValue > 0.5f);
				if(EditorGUI.EndChangeCheck()) {
					propertyX.floatValue = modifiedValue ? 1.0f : 0.0f;
					propertyY.floatValue = 0.0f;
					propertyZ.floatValue = 0.0f;
					propertyW.floatValue = 0.0f;
				}

				break;
			}
			case PixelpartVariantValue.VariantType.Int: {
				EditorGUI.BeginChangeCheck();
				int modifiedValue = EditorGUI.IntField(position, label, (int)propertyX.floatValue);
				if(EditorGUI.EndChangeCheck()) {
					propertyX.floatValue = (float)modifiedValue;
					propertyY.floatValue = 0.0f;
					propertyZ.floatValue = 0.0f;
					propertyW.floatValue = 0.0f;
				}

				break;
			}
			case PixelpartVariantValue.VariantType.Float: {
				EditorGUI.BeginChangeCheck();
				float modifiedValue = EditorGUI.FloatField(position, label, propertyX.floatValue);
				if(EditorGUI.EndChangeCheck()) {
					propertyX.floatValue = modifiedValue;
					propertyY.floatValue = 0.0f;
					propertyZ.floatValue = 0.0f;
					propertyW.floatValue = 0.0f;
				}

				break;
			}
			case PixelpartVariantValue.VariantType.Float2: {
				EditorGUI.BeginChangeCheck();
				Vector2 modifiedValue = EditorGUI.Vector2Field(position, label, new Vector2(
					propertyX.floatValue,
					propertyY.floatValue));
				if(EditorGUI.EndChangeCheck()) {
					propertyX.floatValue = modifiedValue.x;
					propertyY.floatValue = modifiedValue.y;
					propertyZ.floatValue = 0.0f;
					propertyW.floatValue = 0.0f;
				}

				break;
			}
			case PixelpartVariantValue.VariantType.Float3: {
				EditorGUI.BeginChangeCheck();
				Vector3 modifiedValue = EditorGUI.Vector3Field(position, label, new Vector3(
					propertyX.floatValue,
					propertyY.floatValue,
					propertyZ.floatValue));
				if(EditorGUI.EndChangeCheck()) {
					propertyX.floatValue = modifiedValue.x;
					propertyY.floatValue = modifiedValue.y;
					propertyZ.floatValue = modifiedValue.z;
					propertyW.floatValue = 0.0f;
				}

				break;
			}
			case PixelpartVariantValue.VariantType.Float4: {
				EditorGUI.BeginChangeCheck();
				Vector4 modifiedValue = EditorGUI.Vector4Field(position, label, new Vector4(
					propertyX.floatValue,
					propertyY.floatValue,
					propertyZ.floatValue,
					propertyW.floatValue));
				if(EditorGUI.EndChangeCheck()) {
					propertyX.floatValue = modifiedValue.x;
					propertyY.floatValue = modifiedValue.y;
					propertyZ.floatValue = modifiedValue.z;
					propertyW.floatValue = modifiedValue.w;
				}

				break;
			}
			default: {
				break;
			}
		}

		EditorGUI.EndProperty();
	}
}
}
#endif