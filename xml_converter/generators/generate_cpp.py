from dataclasses import dataclass, field
from typing import Set, List, Dict, Optional, Tuple, TypedDict
from jinja2 import FileSystemLoader, Environment, StrictUndefined, Template
from metadata import CustomFunction, MetadataType
from jinja_helpers import UnindentBlocks
from util import lowercase, capitalize, normalize, Document
import os
from protobuf_types import is_proto_field_scalar, get_proto_field_cpp_type, get_proto_field_cpp_prototype


################################################################################
# DocumentationTypeData
#
# A type definition to indicate what information should be included in the
# documentation_type_data variable.
################################################################################
class DocumentationTypeData(TypedDict):
    class_name: str
    cpp_type: str


# A map between the documentation types, and useful class name info related to
# that type.
documentation_type_data: Dict[str, DocumentationTypeData] = {
    "Fixed32": {
        "class_name": "int",
        "cpp_type": "int",
    },
    "Int32": {
        "class_name": "int",
        "cpp_type": "int",
    },
    "Boolean": {
        "class_name": "bool",
        "cpp_type": "bool",
    },
    "Float32": {
        "class_name": "float",
        "cpp_type": "float",
    },
    "String": {
        "class_name": "string",
        "cpp_type": "std::string"
    }
}


@dataclass
class AttributeVariableProtoInfo:
    # The function name and additional side effect pointers for proto serialization.
    serialize_proto_function: str
    serialize_proto_side_effects: List[str]

    # The function name and additional side effect pointers for proto deserialization.
    deserialize_proto_function: str
    deserialize_proto_side_effects: List[str]

    # The name of the field in the protobuf that this attribute corresponds to.
    protobuf_field: str

    # A the cpp type the protobuf library expects to receive.
    protobuf_cpp_type: str

    # Protoc cares if a field is scalar or not in the case of "set" vs "set allocated".
    is_proto_field_scalar: bool

    # The CPP code to inject into the variable getter to drill down to the
    # variable we are looking for. eg ".trigger()" or ".one().two()"
    proto_drilldown_calls: str = ""

    # The CPP code to inject into the variable setter to drill down to the
    # variable we are looking for. eg ".mutable_trigger()" or "mutable_one()->mutable_two()->"
    mutable_proto_drilldown_calls: str = ""


@dataclass
class AttributeVariableXMLInfo:
    xml_fields: List[str]

    # The function name and additional side effect pointers for xml serialization.
    serialize_xml_function: str
    serialize_xml_side_effects: List[str]

    # The function name and additional side effect pointers for xml deserialization.
    deserialize_xml_function: str
    deserialize_xml_side_effects: List[str]

    default_xml_field: str = ""
    write_to_xml: bool = True


@dataclass
class AttributeVariable:
    attribute_name: str
    cpp_type: str
    class_name: str

    proto_info: Optional[AttributeVariableProtoInfo]
    xml_info: AttributeVariableXMLInfo

    attribute_flag_name: Optional[str] = ""

    is_component: bool = False


@dataclass
class AttributeComponent:
    attribute_name: str
    cpp_type: str
    protobuf_field: Optional[str]
    xml_fields: List[str]


################################################################################
# CPPInclude
#
# A series of include paths and forward declarations for a pair of hpp and cpp
# files to use at the top of the file.
################################################################################
@dataclass
class CPPInclude:
    hpp_relative_includes: Set[str] = field(default_factory=set)
    hpp_absolute_includes: Set[str] = field(default_factory=set)
    hpp_forward_declarations: Set[str] = field(default_factory=set)
    cpp_relative_includes: Set[str] = field(default_factory=set)
    cpp_absolute_includes: Set[str] = field(default_factory=set)
    cpp_forward_declarations: Set[str] = field(default_factory=set)

    def sorted_hpp_relative_includes(self) -> List[str]:
        return sorted(self.hpp_relative_includes)

    def sorted_hpp_absolute_includes(self) -> List[str]:
        return sorted(self.hpp_absolute_includes)

    def sorted_hpp_forward_declarations(self) -> List[str]:
        return sorted(self.hpp_forward_declarations)

    def sorted_cpp_relative_includes(self) -> List[str]:
        return sorted(self.cpp_relative_includes)

    def sorted_cpp_absolute_includes(self) -> List[str]:
        return sorted(self.cpp_absolute_includes)

    def sorted_cpp_forward_declarations(self) -> List[str]:
        return sorted(self.cpp_forward_declarations)


############################################################################
# write_cpp_classes
#
# Create all of the auto generated cpp classes that can be created from
# the documents.
############################################################################
def write_cpp_classes(
    output_directory: str,
    data: Dict[str, Document],
) -> List[str]:
    print("Writing XML Node Cpp Classes")

    written_files: List[str] = []

    file_loader = FileSystemLoader('cpp_templates')
    env = Environment(
        loader=file_loader,
        extensions=[UnindentBlocks],
        keep_trailing_newline=True,
        trim_blocks=True,
        lstrip_blocks=True,
        undefined=StrictUndefined
    )
    header_template: Template = env.get_template("class_template.hpp")
    code_template: Template = env.get_template("class_template.cpp")
    attributes_of_type_marker_category: List[str] = []
    cpp_classes: Dict[str, str] = {
        "Category": "MarkerCategory",
        "Icon": "POI",
        "Trail": "Trail"
    }

    for cpp_class in cpp_classes:
        attributes_of_type_marker_category = []

        attribute_variables: List[AttributeVariable]
        attribute_variables, cpp_includes = generate_cpp_variable_data(cpp_class, data)

        for attribute_variable in attribute_variables:
            if attribute_variable.class_name == "marker_category":
                attributes_of_type_marker_category.append(attribute_variable.attribute_name)

        hpp_filepath = os.path.join(output_directory, lowercase(cpp_class) + "_gen.hpp")
        write_if_different(
            path=hpp_filepath,
            contents=header_template.render(
                cpp_class=cpp_class,
                attribute_variables=sorted(attribute_variables, key=get_attribute_variable_key),
                cpp_includes=cpp_includes,
                cpp_class_header=lowercase(cpp_class),
                attributes_of_type_marker_category=attributes_of_type_marker_category,
            ),
        )
        written_files.append(hpp_filepath)

        cpp_filepath = os.path.join(output_directory, lowercase(cpp_class) + "_gen.cpp")
        write_if_different(
            path=cpp_filepath,
            contents=code_template.render(
                cpp_class=cpp_class,
                cpp_includes=cpp_includes,
                cpp_class_header=lowercase(cpp_class),
                xml_class_name=cpp_classes[cpp_class],
                attribute_variables=sorted(attribute_variables, key=get_attribute_variable_key),
                enumerate=enumerate,
                attributes_of_type_marker_category=attributes_of_type_marker_category,
            ),
        )
        written_files.append(cpp_filepath)

    return written_files


################################################################################
# convert_side_effects_to_variable_names
#
# The side effects defined in the markdown documentation files are not the same
# as the internal variables used. This function takes the markdown side effects
# and converts them to thir variable names, it also adds the boolean is_set
# flags as part of the arguments passed.
################################################################################
def convert_side_effects_to_variable_names(side_effects: List[str]) -> List[str]:
    side_effect_variables: List[str] = []

    for side_effect in side_effects:
        side_effect_variables.append(attribute_name_from_markdown_data(side_effect))
        side_effect_variables.append(attribute_name_from_markdown_data(side_effect) + "_is_set")

    return side_effect_variables


############################################################################
# generate_cpp_variable_data
#
# Generate a list of all the variables this cpp class will have as well as
# data about what other files need to be included in the associated hpp and
# cpp files.
############################################################################
def generate_cpp_variable_data(
    doc_type: str,
    data: Dict[str, Document],
) -> Tuple[List[AttributeVariable], CPPInclude]:

    # Type defining the outputs
    attribute_variables: List[AttributeVariable] = []
    cpp_includes: CPPInclude = CPPInclude()

    cpp_includes.hpp_absolute_includes.update([
        "string",
        "vector",
        "functional",
    ])
    cpp_includes.hpp_relative_includes.update([
        "rapidxml-1.13/rapidxml.hpp",
        "parseable.hpp",
        "guildpoint.pb.h",
        "state_structs/xml_reader_state.hpp",
    ])
    cpp_includes.hpp_forward_declarations.update([
        "XMLError",
    ])

    cpp_includes.cpp_absolute_includes.update([
        "iosfwd",
        "string",
    ])
    cpp_includes.cpp_relative_includes.update([
        "rapidxml-1.13/rapidxml.hpp",
        "string_helper.hpp",
        "rapid_helpers.hpp",
        "guildpoint.pb.h",
    ])

    if (doc_type == "Category"):
        cpp_includes.hpp_absolute_includes.update([
            "map",
        ])
        cpp_includes.hpp_relative_includes.update([
            "icon_gen.hpp",
            "trail_gen.hpp",
        ])

        cpp_includes.cpp_absolute_includes.update([
            "utility",
            "type_traits",
        ])

    for filepath, document in sorted(data.items()):
        fieldval = document.metadata
        attribute_name: str = attribute_name_from_markdown_data(fieldval.name)

        if doc_type in fieldval.applies_to_as_str():
            xml_fields: List[str] = []
            write_to_xml: bool = True
            default_xml_field: str = ""

            if fieldval.variable_type in documentation_type_data:
                cpp_type = documentation_type_data[fieldval.variable_type]["cpp_type"]
                class_name = documentation_type_data[fieldval.variable_type]["class_name"]
                cpp_includes.cpp_relative_includes.add("attribute/{}.hpp".format(class_name))

            # elif fieldval.variable_type in ("Custom", "CompoundCustomClass"):
            elif fieldval.variable_type == "Custom" or fieldval.variable_type == "CompoundCustomClass":
                cpp_type = fieldval.cpp_class
                class_name = insert_delimiter(fieldval.cpp_class, delimiter="_")
                cpp_includes.hpp_relative_includes.add("attribute/{}.hpp".format(class_name))

            elif fieldval.variable_type in ("Enum", "MultiflagValue", "CompoundValue"):
                cpp_type = capitalize(attribute_name, delimiter="")
                class_name = attribute_name

                cpp_includes.hpp_relative_includes.add("attribute/{}_gen.hpp".format(class_name))

            else:
                raise ValueError("Unexpected type {field_type} for attribute {attribute_name}".format(
                    field_type=fieldval.variable_type,
                    attribute_name=attribute_name,
                ))

            for x in fieldval.xml_fields:
                xml_fields.append(lowercase(x, delimiter=""))
            default_xml_field = fieldval.xml_fields[0]

            # Compound Values are unique in that the components have xml fields in addition to the compound variable
            # if fieldval.variable_type in ("CompoundValue", "CompoundCustomClass"):
            if fieldval.variable_type == "CompoundValue" or fieldval.variable_type == "CompoundCustomClass":
                for component in fieldval.components:
                    component_xml_fields: List[str] = []
                    component_name: str = attribute_name_from_markdown_data(component.name)
                    component_default_xml_field: str = ""
                    for x in component.xml_fields:
                        component_xml_fields.append(lowercase(x, delimiter=""))
                    component_class_name = documentation_type_data[component.subcomponent_type.value]["class_name"]
                    if component.name in fieldval.xml_separate_components:
                        component_default_xml_field = component.xml_fields[0]
                        write_to_xml = True
                    if component.name in fieldval.xml_bundled_components:
                        component_default_xml_field = fieldval.xml_fields[0]
                        write_to_xml = False
                    component_attribute_variable = AttributeVariable(
                        attribute_name=attribute_name + "." + component_name,
                        cpp_type=documentation_type_data[component.subcomponent_type.value]["cpp_type"],
                        class_name=component_class_name,
                        attribute_flag_name=attribute_name + "_is_set",
                        is_component=True,

                        proto_info=AttributeVariableProtoInfo(
                            protobuf_field=component.protobuf_field,
                            protobuf_cpp_type=get_proto_field_cpp_type(doc_type, combine_fields(fieldval.protobuf_field, component.protobuf_field)),
                            is_proto_field_scalar=is_proto_field_scalar(doc_type, combine_fields(fieldval.protobuf_field, component.protobuf_field)),
                            serialize_proto_function="to_proto_" + component_class_name,
                            serialize_proto_side_effects=[],
                            deserialize_proto_function="from_proto_" + component_class_name,
                            deserialize_proto_side_effects=[],
                        ) if fieldval.protobuf_field is not None and component.protobuf_field is not None else None,

                        xml_info=AttributeVariableXMLInfo(
                            xml_fields=component_xml_fields,
                            default_xml_field=component_default_xml_field,
                            write_to_xml=write_to_xml,
                            serialize_xml_function=component_class_name + "_to_xml_attribute",
                            serialize_xml_side_effects=[],
                            deserialize_xml_function="xml_attribute_to_" + component_class_name,
                            deserialize_xml_side_effects=[],
                        )
                    )
                    attribute_variables.append(component_attribute_variable)
                # If there aren't any components to bundle, we don't want to render the attribute
                if fieldval.xml_bundled_components == []:
                    write_to_xml = False

            # Identify the XML serialize function info
            serialize_xml_function: CustomFunction = CustomFunction(
                function=class_name + "_to_xml_attribute",
                side_effects=[],
            )
            if fieldval.custom_functions.write_xml_trail is not None and doc_type == "Trail":
                serialize_xml_function = fieldval.custom_functions.write_xml_trail
            if fieldval.custom_functions.write_xml_icon is not None and doc_type == "Icon":
                serialize_xml_function = fieldval.custom_functions.write_xml_icon
            elif fieldval.custom_functions.write_xml is not None:
                serialize_xml_function = fieldval.custom_functions.write_xml

            # Identify the XML deserialize function info
            deserialize_xml_function: CustomFunction = CustomFunction(
                function="xml_attribute_to_" + class_name,
                side_effects=[],
            )
            if fieldval.custom_functions.read_xml_trail is not None and doc_type == "Trail":
                deserialize_xml_function = fieldval.custom_functions.read_xml_trail
            if fieldval.custom_functions.read_xml_icon is not None and doc_type == "Icon":
                deserialize_xml_function = fieldval.custom_functions.read_xml_icon
            elif fieldval.custom_functions.read_xml is not None:
                deserialize_xml_function = fieldval.custom_functions.read_xml

            # Identify the proto serialize function info
            serialize_proto_function: CustomFunction = CustomFunction(
                function=class_name + "_to_proto",
                side_effects=[],
            )
            if fieldval.custom_functions.write_proto_trail is not None and doc_type == "Trail":
                serialize_proto_function = fieldval.custom_functions.write_proto_trail
            if fieldval.custom_functions.write_proto_icon is not None and doc_type == "Icon":
                serialize_proto_function = fieldval.custom_functions.write_proto_icon
            elif fieldval.custom_functions.write_proto is not None:
                serialize_proto_function = fieldval.custom_functions.write_proto

            # Identify the proto deserialize function info
            deserialize_proto_function: CustomFunction = CustomFunction(
                function="proto_to_" + class_name,
                side_effects=[],
            )
            if fieldval.custom_functions.read_proto_trail is not None and doc_type == "Trail":
                deserialize_proto_function = fieldval.custom_functions.read_proto_trail
            if fieldval.custom_functions.read_proto_icon is not None and doc_type == "Icon":
                deserialize_proto_function = fieldval.custom_functions.read_proto_icon
            elif fieldval.custom_functions.read_proto is not None:
                deserialize_proto_function = fieldval.custom_functions.read_proto

            proto_info: Optional[AttributeVariableProtoInfo] = None
            if fieldval.protobuf_field is not None:
                proto_drilldown_calls: str
                mutable_proto_drilldown_calls: str
                protobuf_field: str
                proto_drilldown_calls, mutable_proto_drilldown_calls, protobuf_field = split_field_into_drilldown(fieldval.protobuf_field)

                proto_info = AttributeVariableProtoInfo(
                    protobuf_field=protobuf_field,
                    protobuf_cpp_type=get_proto_field_cpp_type(doc_type, fieldval.protobuf_field),
                    is_proto_field_scalar=is_proto_field_scalar(doc_type, fieldval.protobuf_field),
                    proto_drilldown_calls=proto_drilldown_calls,
                    mutable_proto_drilldown_calls=mutable_proto_drilldown_calls,
                    serialize_proto_function=serialize_proto_function.function,
                    serialize_proto_side_effects=convert_side_effects_to_variable_names(serialize_proto_function.side_effects),
                    deserialize_proto_function=deserialize_proto_function.function,
                    deserialize_proto_side_effects=convert_side_effects_to_variable_names(deserialize_proto_function.side_effects),
                )

            attribute_variable = AttributeVariable(
                attribute_name=attribute_name,
                cpp_type=cpp_type,
                class_name=class_name,

                attribute_flag_name=attribute_name + "_is_set",

                proto_info=proto_info,

                xml_info=AttributeVariableXMLInfo(
                    xml_fields=xml_fields,
                    default_xml_field=default_xml_field,
                    write_to_xml=write_to_xml,
                    serialize_xml_function=serialize_xml_function.function,
                    serialize_xml_side_effects=convert_side_effects_to_variable_names(serialize_xml_function.side_effects),
                    deserialize_xml_function=deserialize_xml_function.function,
                    deserialize_xml_side_effects=convert_side_effects_to_variable_names(deserialize_xml_function.side_effects),
                ),
            )
            attribute_variables.append(attribute_variable)

    return attribute_variables, cpp_includes


############################################################################
# write_attributes
#
# Creates the attribute files for attributes that contain multiple values
############################################################################
def write_attribute(output_directory: str, data: Dict[str, Document]) -> List[str]:
    print("Writing attributes")
    files_written: List[str] = []
    os.makedirs(output_directory, exist_ok=True)

    file_loader = FileSystemLoader('cpp_templates')
    env = Environment(
        loader=file_loader,
        extensions=[UnindentBlocks],
        keep_trailing_newline=True,
        trim_blocks=True,
        lstrip_blocks=True,
        undefined=StrictUndefined
    )
    attribute_names: Dict[str, str] = {}
    template: Dict[str, Template] = {
        "MultiflagValue": env.get_template("attribute_template_multiflagvalue.cpp"),
        "CompoundValue": env.get_template("attribute_template_compoundvalue.cpp"),
        "Enum": env.get_template("attribute_template_enum.cpp"),
    }

    for filepath in data.keys():
        filename = os.path.basename(filepath)
        attribute_names[filepath] = filename.split(".md")[0]

    for filepath in attribute_names:
        attribute_components: List[AttributeComponent] = []
        xml_bundled_components: List[str] = []
        attribute_data: MetadataType = data[filepath].metadata
        attribute_name = attribute_name_from_markdown_data(attribute_data.name)

        # Early exit if this attribute is not an attribute to generate code for
        if attribute_data.variable_type not in template:
            continue
        if attribute_data.protobuf_field is None:
            raise ValueError("We dont yet support null protobuf fields for generated attribute classes {}".format(attribute_data))

        proto_field_type: str = ""
        proto_field_prototype: Optional[str] = None
        for marker_type in attribute_data.applies_to_as_str():
            new_type = get_proto_field_cpp_type(marker_type, attribute_data.protobuf_field)
            if proto_field_type != "" and proto_field_type != new_type:
                print("Proto Field type differes between different marker types for ", attribute_data.protobuf_field)
            proto_field_type = new_type

            new_prototype = get_proto_field_cpp_prototype(marker_type, attribute_data.protobuf_field)
            if proto_field_prototype is not None and proto_field_prototype != new_prototype:
                print("Proto Field prototype differes between different marker types for ", attribute_data.protobuf_field)
            proto_field_prototype = new_prototype

        protobuf_field: str
        _, _, protobuf_field = split_field_into_drilldown(attribute_data.protobuf_field)

        if attribute_data.variable_type == "MultiflagValue":
            for flag in attribute_data.flags:
                xml_fields = []
                for item in attribute_data.flags[flag]:
                    xml_fields.append(normalize(item))
                attribute_component = AttributeComponent(
                    attribute_name=flag,
                    cpp_type="bool",
                    xml_fields=xml_fields,
                    protobuf_field=protobuf_field,
                )
                attribute_components.append(attribute_component)

        elif attribute_data.variable_type == "CompoundValue":
            for component in attribute_data.components:
                xml_fields = []
                if component.subcomponent_type.value not in documentation_type_data:
                    raise ValueError("Unexpected type for component. Look at markdown file {attribute_name}".format(
                        attribute_name=attribute_name
                    ))
                component_attribute_name: str = attribute_name_from_markdown_data(component.name)
                for item in component.xml_fields:
                    xml_fields.append(normalize(item))
                if component.name in attribute_data.xml_bundled_components:
                    xml_bundled_components.append(component_attribute_name)
                attribute_component = AttributeComponent(
                    attribute_name=component_attribute_name,
                    cpp_type=documentation_type_data[component.subcomponent_type.value]["cpp_type"],
                    xml_fields=xml_fields,
                    protobuf_field=component.protobuf_field,
                )
                attribute_components.append(attribute_component)

        elif attribute_data.variable_type == "Enum":
            for value in attribute_data.values:
                xml_fields = []
                for item in attribute_data.values[value]:
                    xml_fields.append(normalize(item))
                attribute_component = AttributeComponent(
                    attribute_name=value,
                    cpp_type="str",
                    xml_fields=xml_fields,
                    protobuf_field=protobuf_field,
                )
                attribute_components.append(attribute_component)

        else:
            continue

        hpp_filepath = os.path.join(output_directory, attribute_name + "_gen.hpp")
        write_if_different(
            hpp_filepath,
            env.get_template("attribute_template.hpp").render(
                attribute_name=attribute_name,
                attribute_components=sorted(attribute_components, key=get_attribute_component_key),
                class_name=capitalize(attribute_name, delimiter=""),
                type=attribute_data.variable_type,
                proto_field_cpp_type=proto_field_type,
                proto_field_cpp_type_prototype=proto_field_prototype,
            )
        )
        files_written.append(hpp_filepath)

        cpp_filepath = os.path.join(output_directory, attribute_name + "_gen.cpp")
        write_if_different(
            cpp_filepath,
            template[attribute_data.variable_type].render(
                attribute_name=attribute_name,
                # TODO: Should this attribute_components list be sorted? The hpp one is.
                attribute_components=attribute_components,
                class_name=capitalize(attribute_name, delimiter=""),
                enumerate=enumerate,
                xml_bundled_components=xml_bundled_components,
                proto_field_cpp_type=proto_field_type,
                proto_field_cpp_type_prototype=proto_field_prototype,
            )
        )
        files_written.append(cpp_filepath)

    return files_written


def combine_fields(base_field: str, sub_field: Optional[str]) -> str:
    if sub_field is None:
        return base_field
    return base_field + "." + sub_field


################################################################################
# write_if_different
#
# Writes a file, but only writes it if the file contents is different so that
# no modified timestamps get updated on files that are not changed.
################################################################################
def write_if_different(path: str, contents: str) -> None:
    if os.path.exists(path):
        with open(path, "r") as f:
            data = f.read()
    else:
        data = ""

    if data != contents:
        with open(path, "w") as f:
            f.write(contents)


def get_attribute_variable_key(attribute_variable: AttributeVariable) -> str:
    return attribute_variable.attribute_name


def get_attribute_component_key(attribute_component: AttributeComponent) -> str:
    return attribute_component.attribute_name


################################################################################
# split_field_into_drilldown
#
# Splits the field string into a cpp drilldown function call stack and the
# final proto field name.
# EG:
#   field: "trigger.subclass.fieldname"
#   returns: (".trigger().subclass()", "mutable_trigger()->mutable_subclass()->", "fieldname")
################################################################################
def split_field_into_drilldown(field: str) -> Tuple[str, str, str]:
    components = field.split(".")
    proto_drilldown_calls = "".join([".{}()".format(x) for x in components[:-1]])
    mutable_proto_drilldown_calls = "".join(["mutable_{}()->".format(x) for x in components[:-1]])
    protobuf_field = components[-1]
    return proto_drilldown_calls, mutable_proto_drilldown_calls, protobuf_field


############################################################################
# attribute_name_from_markdown_data
#
# Takes the name of an attribute from the markdown file and returns the name
# that should be used internally to store that value.
############################################################################
def attribute_name_from_markdown_data(attribute_name: str) -> str:
    return lowercase(attribute_name, "_")


################################################################################
# insert_delimiter (Process is called tmesis)
#
# A helper function to take a string with no delmiter and convert it to a snake
# case string of lowercase letters. A delimiter can be passed in to change the
# characters inserted between the newly lowercased words.
################################################################################
def insert_delimiter(word: str, delimiter: str = "_") -> str:
    delimitered_word_array = []

    for i, letter in enumerate(word):
        if letter.isupper():
            if i != 0:
                delimitered_word_array.append(delimiter)
            delimitered_word_array.append(letter.lower())
        else:
            delimitered_word_array.append(letter)

    return "".join(delimitered_word_array)
