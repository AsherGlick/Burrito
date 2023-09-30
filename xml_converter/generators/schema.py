from typing import Optional, List, Dict, Iterable, TypedDict, Literal, Union, Tuple


################################################################################
# A union type of all of the different type helpers found below
################################################################################
DefType = Union[
    "StringDef",
    "BooleanDef",
    "EnumDef",
    "ArrayDef",
    "ObjectDef",
    "PatternDictionaryDef"
]


################################################################################
# String Definition Helpers
#
#
################################################################################
class BaseStringDef(TypedDict):
    type: Literal["string"]


class StringDef(BaseStringDef, total=False):
    pattern: Optional[str]


def string_t(pattern: Optional[str] = None) -> StringDef:
    if pattern is None:
        return {
            "type": "string"
        }
    else:
        return {
            "type": "string",
            "pattern": pattern,
        }


class BooleanDef(TypedDict):
    type: Literal["boolean"]


def boolean_t() -> BooleanDef:
    return {
        "type": "boolean"
    }


class EnumDef(TypedDict):
    type: Literal["string"]
    enum: List[str]


def enum_t(options: Iterable[str]) -> EnumDef:
    return {
        "type": "string",
        "enum": list(options)
    }


class ArrayDef(TypedDict):
    type: Literal["array"]
    items: DefType


def array_t(element_type: DefType) -> ArrayDef:
    return {
        "type": "array",
        "items": element_type
    }


class ObjectDef(TypedDict):
    type: Literal["object"]
    additionalProperties: Literal[False]
    required: List[str]
    properties: Dict[str, DefType]


def object_t(fields: Dict[str, DefType], optional_fields: Dict[str, DefType] = {}) -> ObjectDef:
    return {
        "type": "object",
        "additionalProperties": False,
        "required": list(fields.keys()),
        "properties": {**fields, **optional_fields}
    }


class PatternDictionaryDef(TypedDict):
    type: Literal["object"]
    patternProperties: Dict[str, DefType]


def pattern_dictionary_t(pattern_properties: Dict[str, DefType]) -> PatternDictionaryDef:
    return {
        "type": "object",
        "patternProperties": pattern_properties
    }


# Helper function for the union types
def union_partial_t(
    *,
    required: Dict[str, DefType] = {},
    optional: Dict[str, DefType] = {}
) -> Tuple[Dict[str, DefType], Dict[str, DefType]]:
    return (required, optional)


class UnionBranchThenDef(TypedDict):
    additionalProperties: Literal[False]
    required: List[str]
    properties: Dict[str, DefType]


UnionBranchDef = TypedDict('UnionBranchDef', {
    'if': Dict[Literal["properties"], Dict[Literal["type"], Dict[Literal["const"], str]]],
    'then': UnionBranchThenDef,
})


class UnionDef(TypedDict):
    type: Literal["object"]
    properties: Dict[Literal["type"], EnumDef]
    allOf: List[UnionBranchDef]


def union_t(options: Dict[str, Tuple[Dict[str, DefType], Dict[str, DefType]]]) -> UnionDef:
    union_type: UnionDef = {
        "type": "object",
        "properties": {
            "type": enum_t(options.keys())
        },
        "allOf": [
            {
                "if": {
                    "properties": {
                        "type": {
                            "const": key
                        }
                    }
                },
                "then": {
                    "additionalProperties": False,
                    "required": list(value[0].keys()),
                    "properties": {**value[0], **value[1]}
                }
            }
            for key, value in options.items()
        ]
    }

    return union_type


shared_field_properties: Dict[str, DefType] = {
    "type": string_t(),
    "name": string_t(),
    "applies_to": array_t(enum_t(["Icon", "Trail", "Category"])),
    # To Be Depricated
    "compatability": array_t(enum_t(["BlishHUD", "Burrito", "TacO"])),
    "xml_fields": array_t(string_t(pattern="^[A-Za-z]+$")),
    "protobuf_field": string_t(pattern="^[a-z_.]+$"),
}

schema = union_t({
    "Int32": union_partial_t(required=shared_field_properties),
    "Fixed32": union_partial_t(required=shared_field_properties),
    "Float32": union_partial_t(required=shared_field_properties),
    "String": union_partial_t(required=shared_field_properties),
    "Boolean": union_partial_t(required=shared_field_properties),
    "MultiflagValue": union_partial_t(
        required={**shared_field_properties, **{
            "flags": pattern_dictionary_t({"^[a-z_]+$": array_t(string_t())}),
        }},
    ),
    "Enum": union_partial_t(
        required={**shared_field_properties, **{
            "values": pattern_dictionary_t({"^[a-z_]+$": array_t(string_t())})
        }}
    ),
    "CompoundValue": union_partial_t(
        required={**shared_field_properties, **{
            "xml_bundled_components": array_t(string_t()),
            "xml_separate_components": array_t(string_t()),
            "components": array_t(object_t({
                "name": string_t(),
                "type": enum_t(["Int32", "Fixed32", "Float32"]),
                "xml_fields": array_t(string_t("^[A-Za-z]+$")),
                "protobuf_field": string_t("^[a-z_.]+$"),
                # To Be Depricated
                "compatability": array_t(enum_t(["BlishHUD", "Burrito", "TacO"]))
            })),
        }}
    ),

    "CompoundCustomClass": union_partial_t(
        required={**shared_field_properties, **{
            "class": string_t(),
            "xml_bundled_components": array_t(string_t()),
            "xml_separate_components": array_t(string_t()),
            "components": array_t(object_t({
                "name": string_t(),
                "type": enum_t(["Int32", "Fixed32", "Float32"]),
                "xml_fields": array_t(string_t("^[A-Za-z]+$")),
                "protobuf_field": string_t("^[a-z_.]+$"),
                # To Be Depricated
                "compatability": array_t(enum_t(["BlishHUD", "Burrito", "TacO"]))
            })),
        }}
    ),

    "Custom": union_partial_t(
        required={**shared_field_properties, **{"class": string_t()}},
        optional={
            "side_effects": array_t(string_t()),
            "uses_file_path": boolean_t(),
        }
    ),
})
