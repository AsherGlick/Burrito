from dataclasses import dataclass, field
import enum
from typing import Dict, List, Literal, Optional, Any, Union, Final
import classnotation

XML_ATTRIBUTE_REGEX: Final[str] = "^[A-Za-z]+$"
PROTO_FIELD_REGEX: Final[str] = "^[a-z_.]+$"
INTERNAL_VARIABLE_REGEX: Final[str] = "^[a-z_]+$"
ATTRIBUTE_NAME_REGEX: Final[str] = "^[A-Za-z ]+$"


class NodeType(enum.Enum):
    icon = "Icon"
    trail = "Trail"
    category = "Category"


class SubcomponentType(enum.Enum):
    int32 = "Int32"
    fixed32 = "Fixed32"
    float32 = "Float32"


@dataclass
class CustomFunction:
    function: str
    side_effects: List[str]


@dataclass
class CustomFunctions:
    read_xml: Optional[CustomFunction] = field(default=None, metadata={"json": "read.xml"})
    read_xml_icon: Optional[CustomFunction] = field(default=None, metadata={"json": "read.xml.icon"})
    read_xml_trail: Optional[CustomFunction] = field(default=None, metadata={"json": "read.xml.trail"})

    read_proto: Optional[CustomFunction] = field(default=None, metadata={"json": "read.proto"})
    read_proto_icon: Optional[CustomFunction] = field(default=None, metadata={"json": "read.proto.icon"})
    read_proto_trail: Optional[CustomFunction] = field(default=None, metadata={"json": "read.proto.trail"})

    write_xml: Optional[CustomFunction] = field(default=None, metadata={"json": "write.xml"})
    write_xml_icon: Optional[CustomFunction] = field(default=None, metadata={"json": "write.xml.icon"})
    write_xml_trail: Optional[CustomFunction] = field(default=None, metadata={"json": "write.xml.trail"})

    write_proto: Optional[CustomFunction] = field(default=None, metadata={"json": "write.proto"})
    write_proto_icon: Optional[CustomFunction] = field(default=None, metadata={"json": "write.proto.icon"})
    write_proto_trail: Optional[CustomFunction] = field(default=None, metadata={"json": "write.proto.trail"})


@dataclass
class BaseMetadata:
    name: str  # TODO Match this to the regex ATTRIBUTE_NAME_REGEX
    applies_to: List[NodeType]
    xml_fields: List[str]  # TODO: Matche these to XML_ATTRIBUTE_REGEX
    protobuf_field: Optional[str]  # TODO: Match this to PROTO_FIELD_REGEX

    def applies_to_as_str(self) -> List[str]:
        return [x.value for x in self.applies_to]


@dataclass
class OptionalBaseMetadata:
    custom_functions: CustomFunctions = CustomFunctions()
    examples: List[str] = field(default_factory=list)


################################################################################
# Int32Metadata
################################################################################
@dataclass
class _Int32Metadata:
    variable_type: Literal["Int32"] = field(metadata={"json": "type"})


@dataclass
class Int32Metadata(OptionalBaseMetadata, _Int32Metadata, BaseMetadata):
    pass


################################################################################
# Fixed32Metadata
################################################################################
@dataclass
class _Fixed32Metadata:
    variable_type: Literal["Fixed32"] = field(metadata={"json": "type"})


@dataclass
class Fixed32Metadata(OptionalBaseMetadata, _Fixed32Metadata, BaseMetadata):
    pass


################################################################################
# Float32Metadata
################################################################################
@dataclass
class _Float32Metadata:
    variable_type: Literal["Float32"] = field(metadata={"json": "type"})


@dataclass
class Float32Metadata(OptionalBaseMetadata, _Float32Metadata, BaseMetadata):
    pass


################################################################################
# StringMetadata
################################################################################
@dataclass
class _StringMetadata:
    variable_type: Literal["String"] = field(metadata={"json": "type"})


@dataclass
class StringMetadata(OptionalBaseMetadata, _StringMetadata, BaseMetadata):
    pass


################################################################################
# BooleanMetadata
################################################################################
@dataclass
class _BooleanMetadata:
    variable_type: Literal["Boolean"] = field(metadata={"json": "type"})


@dataclass
class BooleanMetadata(OptionalBaseMetadata, _BooleanMetadata, BaseMetadata):
    pass


################################################################################
# MultiFlagValueMetadata
################################################################################
@dataclass
class _MultiFlagValueMetadata:
    variable_type: Literal["MultiflagValue"] = field(metadata={"json": "type"})
    flags: Dict[str, List[str]]  # Validate keys against INTERNAL_VARIBLE_REGEX


@dataclass
class MultiFlagValueMetadata(OptionalBaseMetadata, _MultiFlagValueMetadata, BaseMetadata):
    pass


################################################################################
# EnumMetadata
################################################################################
@dataclass
class _EnumMetadata:
    variable_type: Literal["Enum"] = field(metadata={"json": "type"})
    values: Dict[str, List[str]]  # Validate keys against INTERNAL_VARIBLE_REGEX


@dataclass
class EnumMetadata(OptionalBaseMetadata, _EnumMetadata, BaseMetadata):
    pass


################################################################################
# CompoundValueMetadata
################################################################################
@dataclass
class CompoundSubComponent:
    name: str
    subcomponent_type: SubcomponentType = field(metadata={"json": "type"})
    xml_fields: List[str]
    protobuf_field: Optional[str]
    examples: List[str] = field(default_factory=list)


@dataclass
class _CompoundValueMetadata:
    variable_type: Literal["CompoundValue"] = field(metadata={"json": "type"})
    xml_bundled_components: List[str]
    xml_separate_components: List[str]

    components: List[CompoundSubComponent]


@dataclass
class CompoundValueMetadata(OptionalBaseMetadata, _CompoundValueMetadata, BaseMetadata):
    pass


################################################################################
# CompoundCustomClassMetadata
################################################################################
@dataclass
class _CompoundCustomClassMetadata:
    variable_type: Literal["CompoundCustomClass"] = field(metadata={"json": "type"})
    cpp_class: str = field(metadata={"json": "class"})
    xml_bundled_components: List[str]
    xml_separate_components: List[str]
    components: List[CompoundSubComponent]


@dataclass
class CompoundCustomClassMetadata(OptionalBaseMetadata, _CompoundCustomClassMetadata, BaseMetadata):
    pass


################################################################################
# CustomMetadata
################################################################################
@dataclass
class _CustomMetadata:
    variable_type: Literal["Custom"] = field(metadata={"json": "type"})
    cpp_class: str = field(metadata={"json": "class"})
    uses_file_path: bool = False


@dataclass
class CustomMetadata(OptionalBaseMetadata, _CustomMetadata, BaseMetadata):
    pass


MetadataType = Union[
    Int32Metadata,
    Fixed32Metadata,
    Float32Metadata,
    StringMetadata,
    BooleanMetadata,
    MultiFlagValueMetadata,
    EnumMetadata,
    CompoundValueMetadata,
    CompoundCustomClassMetadata,
    CustomMetadata
]


@dataclass
class Wrapper:
    metadata: MetadataType


def parse_data(document: Any) -> MetadataType:
    wrapper: Wrapper = classnotation.load_data(Wrapper, {"metadata": document})
    return wrapper.metadata
