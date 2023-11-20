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
# These are used as helpers for string_t() which is a helper for creating a
# "string" jsonschema definition object. Optionally string_t() can be called
# with a regex pattern that will be applied to the json schema.
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


################################################################################
# Boolean Definition Helpers
#
# These are helpers for creating a "boolean" jsonschema definition object.
# boolean_t() does not take any arguments and will always return the same value
################################################################################
class BooleanDef(TypedDict):
    type: Literal["boolean"]


def boolean_t() -> BooleanDef:
    return {
        "type": "boolean"
    }


################################################################################
# Enumeration Definition Helpers
#
# These are helpers for creating "enum" jsonschema definition objects. These
# are string values that can only be one of a set number of values. The values
# that are allowed are passed into the enum_t() function.
################################################################################
class EnumDef(TypedDict):
    type: Literal["string"]
    enum: List[str]


def enum_t(options: Iterable[str]) -> EnumDef:
    return {
        "type": "string",
        "enum": list(options)
    }


################################################################################
# Array Definition Helpers
#
# Theses are helpers for creating "array" jsonschema definition objects. Arrays
# Take in a subtype that represents the type of their elements.
################################################################################
class ArrayDef(TypedDict):
    type: Literal["array"]
    items: DefType


def array_t(element_type: DefType) -> ArrayDef:
    return {
        "type": "array",
        "items": element_type
    }


################################################################################
# Object Definition Helpers
#
# These are helpers for creating object jsonschema definitions. Objects contain
# both required and optional fields. Objects in jsonschema normally just
# denote fields that might exist, but dont force them to exist or prevent other
# fields from existing by default. This helper automatically forces all of the
# required fields to exist, and restricts any non-required non-optional field.
################################################################################
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


################################################################################
# Pattern Dictionary Definition Helpers
#
# These are helpers for creating dictionary types that have pattern
# requirements on their keys. This is a special type for jsonschema and it
# may be replaced in the future with something else given its uniqueness.
################################################################################
class PatternDictionaryDef(TypedDict):
    type: Literal["object"]
    patternProperties: Dict[str, DefType]


def pattern_dictionary_t(pattern_properties: Dict[str, DefType]) -> PatternDictionaryDef:
    return {
        "type": "object",
        "patternProperties": pattern_properties
    }


################################################################################
# Union Definition Type
#
# These are helpers for creating union types in jsonschema. Unions seem to be
# very difficult to accomplish in jsonschema but union_t() will abstract all
# of the complexities away, only requiring a map of strings to required and
# optional fields, similar to an object.
################################################################################
# Helper function for the union types to more easily write required/optional
# tuples inline.
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
