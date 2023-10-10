from jsonschema import validate  # type:ignore
from jsonschema.exceptions import ValidationError  # type:ignore
import frontmatter  # type:ignore
from typing import Any, Dict, List, Tuple, Set, Optional, Final, TypedDict
import os
import markdown
from dataclasses import dataclass, field
from jinja2 import Template, FileSystemLoader, Environment
from jinja_helpers import UnindentBlocks
from schema import string_t, array_t, enum_t, union_t, union_partial_t, pattern_dictionary_t, object_t, boolean_t, DefType


SchemaType = Dict[str, Any]

XML_ATTRIBUTE_REGEX: Final[str] = "^[A-Za-z]+$"
PROTO_FIELD_REGEX: Final[str] = "^[a-z_.]+$"
INTERNAL_VARIABLE_REGEX: Final[str] = "^[a-z_]+$"

shared_field_properties: Dict[str, DefType] = {
    "type": string_t(),
    "name": string_t(),
    "applies_to": array_t(enum_t(["Icon", "Trail", "Category"])),
    "xml_fields": array_t(string_t(pattern=XML_ATTRIBUTE_REGEX)),
    "protobuf_field": string_t(pattern=PROTO_FIELD_REGEX),
}

schema = union_t({
    "Int32": union_partial_t(required=shared_field_properties),
    "Fixed32": union_partial_t(required=shared_field_properties),
    "Float32": union_partial_t(required=shared_field_properties),
    "String": union_partial_t(required=shared_field_properties),
    "Boolean": union_partial_t(required=shared_field_properties),
    "MultiflagValue": union_partial_t(
        required={**shared_field_properties, **{
            "flags": pattern_dictionary_t({INTERNAL_VARIABLE_REGEX: array_t(string_t())}),
        }},
    ),
    "Enum": union_partial_t(
        required={**shared_field_properties, **{
            "values": pattern_dictionary_t({INTERNAL_VARIABLE_REGEX: array_t(string_t())})
        }}
    ),
    "CompoundValue": union_partial_t(
        required={**shared_field_properties, **{
            "xml_bundled_components": array_t(string_t()),
            "xml_separate_components": array_t(string_t()),
            "components": array_t(object_t({
                "name": string_t(),
                "type": enum_t(["Int32", "Fixed32", "Float32"]),
                "xml_fields": array_t(string_t(XML_ATTRIBUTE_REGEX)),
                "protobuf_field": string_t(PROTO_FIELD_REGEX),
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
                "xml_fields": array_t(string_t(XML_ATTRIBUTE_REGEX)),
                "protobuf_field": string_t(PROTO_FIELD_REGEX),
            })),
        }}
    ),
    "Custom": union_partial_t(
        required={**shared_field_properties, **{"class": string_t()}},
        optional={
            "side_effects": array_t(string_t()),
            "uses_file_path": boolean_t(),
            "protobuf_type": enum_t(["Int32", "Fixed32", "Float32", "String"]),
        }
    ),
})


def validate_front_matter_schema(front_matter: Any) -> str:
    try:
        validate(front_matter, schema)
    except ValidationError as e:
        return "Error Message: {} (Path: {}".format(e.message, e.json_path)
    return ""


@dataclass
class Document:
    metadata: SchemaType
    content: str


@dataclass
class FieldRow:
    name: str
    xml_attribute: str
    alternate_xml_attributes: List[str]
    binary_field: str
    data_type: str
    usable_on_html: str
    example: str
    valid_values_html: str
    is_sub_field: bool
    sub_fields: List["FieldRow"]
    description: str


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
class AttributeVariable:
    attribute_name: str
    attribute_type: str
    cpp_type: str
    class_name: str
    xml_fields: List[str]
    protobuf_field: str
    args: List[str] = field(default_factory=list)
    default_xml_field: str = ""
    side_effects: List[str] = field(default_factory=list)
    xml_bundled_components: List[str] = field(default_factory=list)
    attribute_flag_name: Optional[str] = ""
    write_to_xml: bool = True

    # The CPP code to inject into the variable getter to drill down to the
    # variable we are looking for. eg ".trigger()" or ".one().two()"
    proto_drilldown_calls: str = ""

    # The CPP code to inject into the variable setter to drill down to the
    # variable we are looking for. eg ".mutable_trigger()" or "mutable_one()->mutable_two()->"
    mutable_proto_drilldown_calls: str = ""

    uses_file_path: bool = False
    is_component: bool = False

    # A flag to override the type that should be used when writing or reading from a protobuf
    protobuf_type: str = ""

    def __post_init__(self) -> None:
        if self.protobuf_type == "":
            self.protobuf_type = self.attribute_type


XML_ATTRIBUTE_PARSER_DEFAULT_ARGUMENTS: Final[List[str]] = ["attribute", "errors"]


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


def get_attribute_variable_key(attribute_variable: AttributeVariable) -> str:
    return attribute_variable.attribute_name


class Generator:
    data: Dict[str, Document] = {}

    def delete_generated_docs(self, dir_path: str) -> None:
        for filepath in os.listdir(dir_path):
            filepath = os.path.join(dir_path, filepath)
            if filepath.endswith("_gen.hpp") or filepath.endswith("_gen.cpp") or filepath.endswith(".html"):
                os.remove(filepath)

    def load_input_doc(self, dir_path: str) -> None:
        for filepath in os.listdir(dir_path):
            filepath = os.path.join(dir_path, filepath)
            try:
                document = frontmatter.load(filepath)
            except Exception as e:
                print(filepath)
                raise e

            error = validate_front_matter_schema(document.metadata)
            if error != "":
                print(filepath)
                print(error)

            self.data[filepath] = Document(
                metadata=document.metadata,
                content=document.content
            )

    ############################################################################
    # write_cpp_classes
    #
    # Create all of the auto generated cpp classes that can be created from
    # the documents.
    ############################################################################
    def write_cpp_classes(self, output_directory: str) -> None:
        print("Writing XML Node Cpp Classes")
        file_loader = FileSystemLoader('cpp_templates')
        env = Environment(
            loader=file_loader,
            extensions=[UnindentBlocks],
            keep_trailing_newline=True,
            trim_blocks=True,
            lstrip_blocks=True
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
            attribute_variables, cpp_includes = self.generate_cpp_variable_data(cpp_class)

            for attribute_variable in attribute_variables:
                if attribute_variable.class_name == "marker_category":
                    attributes_of_type_marker_category.append(attribute_variable.attribute_name)

            with open(os.path.join(output_directory, lowercase(cpp_class) + "_gen.hpp"), 'w') as f:
                f.write(header_template.render(
                    cpp_class=cpp_class,
                    attribute_variables=sorted(attribute_variables, key=get_attribute_variable_key),
                    cpp_includes=cpp_includes,
                    cpp_class_header=lowercase(cpp_class),
                    attributes_of_type_marker_category=attributes_of_type_marker_category,
                ))

            with open(os.path.join(output_directory, lowercase(cpp_class) + "_gen.cpp"), 'w') as f:
                f.write(code_template.render(
                    cpp_class=cpp_class,
                    cpp_includes=cpp_includes,
                    cpp_class_header=lowercase(cpp_class),
                    xml_class_name=cpp_classes[cpp_class],
                    attribute_variables=sorted(attribute_variables, key=get_attribute_variable_key),
                    enumerate=enumerate,
                    attributes_of_type_marker_category=attributes_of_type_marker_category,
                ))

    ############################################################################
    # generate_cpp_variable_data
    #
    # Generate a list of all the variables this cpp class will have as well as
    # data about what other files need to be included in the associated hpp and
    # cpp files.
    ############################################################################
    def generate_cpp_variable_data(
        self,
        doc_type: str,
    ) -> Tuple[List[AttributeVariable], CPPInclude]:

        # Type defining the outputs
        attribute_variables: List[AttributeVariable] = []
        cpp_includes: CPPInclude = CPPInclude()

        cpp_includes.hpp_absolute_includes.add("string")
        cpp_includes.hpp_absolute_includes.add("vector")
        cpp_includes.hpp_relative_includes.add("rapidxml-1.13/rapidxml.hpp")
        cpp_includes.hpp_relative_includes.add("parseable.hpp")
        cpp_includes.hpp_relative_includes.add("waypoint.pb.h")
        cpp_includes.hpp_forward_declarations.add("XMLError")

        cpp_includes.cpp_absolute_includes.add("iosfwd")
        cpp_includes.cpp_absolute_includes.add("string")
        cpp_includes.cpp_relative_includes.add("rapidxml-1.13/rapidxml.hpp")
        cpp_includes.cpp_relative_includes.add("string_helper.hpp")
        cpp_includes.cpp_relative_includes.add("rapid_helpers.hpp")
        cpp_includes.cpp_relative_includes.add("waypoint.pb.h")

        if (doc_type == "Category"):
            cpp_includes.hpp_absolute_includes.add("map")
            cpp_includes.hpp_relative_includes.add("icon_gen.hpp")
            cpp_includes.hpp_relative_includes.add("trail_gen.hpp")

            cpp_includes.cpp_absolute_includes.add("utility")
            cpp_includes.cpp_absolute_includes.add("type_traits")

        for filepath, document in sorted(self.data.items()):
            fieldval = document.metadata
            attribute_name: str = attribute_name_from_markdown_data(fieldval['name'])

            if doc_type in fieldval['applies_to']:
                xml_fields: List[str] = []
                side_effects: List[str] = []
                write_to_xml: bool = True
                default_xml_field: str = ""

                args: List[str] = XML_ATTRIBUTE_PARSER_DEFAULT_ARGUMENTS.copy()

                if fieldval['type'] in documentation_type_data:
                    cpp_type = documentation_type_data[fieldval['type']]["cpp_type"]
                    class_name = documentation_type_data[fieldval['type']]["class_name"]
                    cpp_includes.cpp_relative_includes.add("attribute/{}.hpp".format(class_name))

                elif fieldval['type'] in ["Custom", "CompoundCustomClass"]:
                    cpp_type = fieldval['class']
                    class_name = insert_delimiter(fieldval['class'], delimiter="_")
                    cpp_includes.hpp_relative_includes.add("attribute/{}.hpp".format(class_name))

                elif fieldval['type'] in ["Enum", "MultiflagValue", "CompoundValue"]:
                    cpp_type = capitalize(attribute_name, delimiter="")
                    class_name = attribute_name

                    cpp_includes.hpp_relative_includes.add("attribute/{}_gen.hpp".format(class_name))

                else:
                    raise ValueError("Unexpected type {field_type} for attribute {attribute_name}".format(
                        field_type=fieldval['type'],
                        attribute_name=attribute_name,
                    ))

                for x in fieldval['xml_fields']:
                    xml_fields.append(lowercase(x, delimiter=""))
                default_xml_field = fieldval['xml_fields'][0]

                proto_drilldown_calls: str
                mutable_proto_drilldown_calls: str
                protobuf_field: str
                proto_drilldown_calls, mutable_proto_drilldown_calls, protobuf_field = split_field_into_drilldown(fieldval["protobuf_field"])

                if fieldval.get("uses_file_path", False):
                    args.append("base_dir")

                if "side_effects" in fieldval:
                    for side_effect in fieldval['side_effects']:
                        side_effects.append(attribute_name_from_markdown_data(side_effect))

                # Compound Values are unique in that the components have xml fields in addition to the compound variable
                if fieldval['type'] in ["CompoundValue", "CompoundCustomClass"]:
                    for component in fieldval['components']:
                        component_xml_fields: List[str] = []
                        component_name: str = attribute_name_from_markdown_data(component['name'])
                        component_default_xml_field: str = ""
                        for x in component['xml_fields']:
                            component_xml_fields.append(lowercase(x, delimiter=""))
                        component_class_name = documentation_type_data[component['type']]["class_name"]
                        if component['name'] in fieldval['xml_separate_components']:
                            component_default_xml_field = component['xml_fields'][0]
                            write_to_xml = True
                        if component['name'] in fieldval['xml_bundled_components']:
                            component_default_xml_field = fieldval['xml_fields'][0]
                            write_to_xml = False
                        component_attribute_variable = AttributeVariable(
                            attribute_name=attribute_name + "." + component_name,
                            attribute_type="CompoundValue",
                            cpp_type=documentation_type_data[component['type']]["cpp_type"],
                            class_name=component_class_name,
                            xml_fields=component_xml_fields,
                            default_xml_field=component_default_xml_field,
                            protobuf_field=component["protobuf_field"],
                            attribute_flag_name=attribute_name + "_is_set",
                            write_to_xml=write_to_xml,
                            is_component=True,
                            args=args,
                        )
                        attribute_variables.append(component_attribute_variable)
                    # If there aren't any components to bundle, we don't want to render the attribute
                    if fieldval['xml_bundled_components'] == []:
                        write_to_xml = False

                protobuf_type = ""
                if "protobuf_type" in fieldval:
                    protobuf_type = fieldval["protobuf_type"]

                attribute_variable = AttributeVariable(
                    attribute_name=attribute_name,
                    attribute_type=fieldval["type"],
                    cpp_type=cpp_type,
                    class_name=class_name,
                    xml_fields=xml_fields,
                    default_xml_field=default_xml_field,
                    protobuf_field=protobuf_field,
                    proto_drilldown_calls=proto_drilldown_calls,
                    mutable_proto_drilldown_calls=mutable_proto_drilldown_calls,
                    args=args,
                    write_to_xml=write_to_xml,
                    attribute_flag_name=attribute_name + "_is_set",
                    side_effects=side_effects,
                    protobuf_type=protobuf_type
                )
                attribute_variables.append(attribute_variable)

        return attribute_variables, cpp_includes

    ############################################################################
    # write_attributes
    #
    # Creates the attribute files for attributes that contain multiple values
    ############################################################################
    def write_attribute(self, output_directory: str) -> None:
        print("Writing attributes")
        os.makedirs(output_directory, exist_ok=True)

        file_loader = FileSystemLoader('cpp_templates')
        env = Environment(
            loader=file_loader,
            extensions=[UnindentBlocks],
            keep_trailing_newline=True,
            trim_blocks=True,
            lstrip_blocks=True
        )
        attribute_names: Dict[str, str] = {}
        attribute_variables: List[AttributeVariable] = []
        attribute_variable: AttributeVariable
        metadata: Dict[str, SchemaType] = {}
        xml_fields: List[str] = []
        template: Dict[str, Template] = {
            "MultiflagValue": env.get_template("multiflagvalue.cpp"),
            "CompoundValue": env.get_template("compoundvalue.cpp"),
            "Enum": env.get_template("enum.cpp"),
        }

        for filepath in self.data.keys():
            filename = os.path.basename(filepath)
            attribute_names[filepath] = filename.split(".md")[0]

        for filepath in attribute_names:
            attribute_variables = []
            xml_bundled_components: List[str] = []
            metadata[filepath] = self.data[filepath].metadata
            attribute_name = attribute_name_from_markdown_data(metadata[filepath]['name'])

            proto_drilldown_calls: str
            mutable_proto_drilldown_calls: str
            protobuf_field: str
            proto_drilldown_calls, mutable_proto_drilldown_calls, protobuf_field = split_field_into_drilldown(metadata[filepath]["protobuf_field"])

            if metadata[filepath]['type'] == "MultiflagValue":
                for flag in metadata[filepath]['flags']:
                    xml_fields = []
                    for item in metadata[filepath]['flags'][flag]:
                        xml_fields.append(normalize(item))

                    attribute_variable = AttributeVariable(
                        attribute_name=flag,
                        attribute_type=metadata[filepath]['type'],
                        cpp_type="bool",
                        class_name=attribute_name,
                        xml_fields=xml_fields,
                        protobuf_field=protobuf_field,
                        proto_drilldown_calls=proto_drilldown_calls,
                        mutable_proto_drilldown_calls=mutable_proto_drilldown_calls
                    )
                    attribute_variables.append(attribute_variable)

            elif metadata[filepath]['type'] == "CompoundValue":
                for component in metadata[filepath]['components']:
                    xml_fields = []
                    if component['type'] not in documentation_type_data:
                        raise ValueError("Unexpected type for component. Look at markdown file {attribute_name}".format(
                            attribute_name=attribute_name
                        ))
                    component_attribute_name: str = attribute_name_from_markdown_data(component['name'])
                    for item in component['xml_fields']:
                        xml_fields.append(normalize(item))
                    if component['name'] in metadata[filepath]['xml_bundled_components']:
                        xml_bundled_components.append(component_attribute_name)
                    attribute_variable = AttributeVariable(
                        attribute_name=component_attribute_name,
                        attribute_type=metadata[filepath]['type'],
                        cpp_type=documentation_type_data[component['type']]["cpp_type"],
                        class_name=attribute_name,
                        xml_fields=xml_fields,
                        protobuf_field=component["protobuf_field"],
                        proto_drilldown_calls=proto_drilldown_calls,
                        mutable_proto_drilldown_calls=mutable_proto_drilldown_calls
                    )
                    attribute_variables.append(attribute_variable)

            elif metadata[filepath]['type'] == "Enum":
                for value in metadata[filepath]['values']:
                    xml_fields = []
                    for item in metadata[filepath]['values'][value]:
                        xml_fields.append(normalize(item))
                    attribute_variable = AttributeVariable(
                        attribute_name=value,
                        attribute_type=metadata[filepath]['type'],
                        cpp_type="str",
                        class_name=attribute_name,
                        xml_fields=xml_fields,
                        protobuf_field=protobuf_field,
                        proto_drilldown_calls=proto_drilldown_calls,
                        mutable_proto_drilldown_calls=mutable_proto_drilldown_calls
                    )
                    attribute_variables.append(attribute_variable)

            else:
                continue

            with open(os.path.join(output_directory, attribute_name + "_gen.hpp"), 'w') as f:
                f.write(env.get_template("attribute_template.hpp").render(
                    attribute_name=attribute_name,
                    attribute_variables=sorted(attribute_variables, key=get_attribute_variable_key),
                    class_name=capitalize(attribute_name, delimiter=""),
                    type=metadata[filepath]['type'],
                ))

            with open(os.path.join(output_directory, attribute_name + "_gen.cpp"), 'w') as f:
                f.write(template[metadata[filepath]['type']].render(
                    attribute_name=attribute_name,
                    attribute_variables=attribute_variables,
                    class_name=capitalize(attribute_name, delimiter=""),
                    enumerate=enumerate,
                    xml_bundled_components=xml_bundled_components
                ))

    ############################################################################
    # write_webdocs
    #
    # Create all of the html pages that can be created from
    # the documents.
    ############################################################################
    def write_webdocs(self, output_directory: str) -> None:
        print("Writing output documentation")
        os.makedirs(output_directory, exist_ok=True)

        file_loader = FileSystemLoader('web_templates')
        env = Environment(loader=file_loader)
        template = env.get_template("documentation.html")
        categories: Dict[str, List[str]] = {}

        for filename in self.data.keys():
            category = os.path.basename(os.path.dirname(filename))
            if category not in categories:
                categories[category] = []
            categories[category].append(filename)

        navigation_links = [(capitalize(category), category) for category in sorted(categories.keys())]

        complete_field_row_list = []

        for page in sorted(categories):
            content: Dict[str, str] = {}
            metadata: Dict[str, SchemaType] = {}
            # Resets the content and metadata to empty for each loop

            for subpage in categories[page]:
                content[subpage] = markdown.markdown(self.data[subpage].content)
                metadata[subpage] = self.data[subpage].metadata

            generated_doc, field_rows = self.generate_auto_docs(metadata, content)

            for field_row in field_rows:
                complete_field_row_list.append(field_row)

            with open(os.path.join(output_directory, page + ".html"), 'w') as f:

                f.write(template.render(
                    generated_doc=generated_doc,
                    content_nav=navigation_links
                ))

    def get_examples(self, field_type: str) -> List[str]:
        return ["???"]

    def get_fixed_option_examples(self, field_type: str, pairings: Any) -> List[str]:
        example_values = []
        for elem_name in pairings:
            example_values.append(pairings[elem_name][0])

        solo_field = ""
        combo_field = []
        if len(example_values) == 1 or field_type == "Enum":
            solo_field = example_values[0]

        elif len(example_values) == 2:
            solo_field = example_values[0]
            combo_field = [example_values[0], example_values[1]]

        elif len(example_values) == 3:
            solo_field = example_values[0]
            combo_field = [example_values[0], example_values[1], example_values[2]]

        elif len(example_values) > 3:
            solo_field = example_values[0]
            combo_field = [example_values[1], example_values[2], example_values[3]]

        examples = []
        examples.append("\"" + solo_field + "\"")
        if len(combo_field) > 0:
            examples.append("\"" + ",".join(combo_field) + "\"")

        return examples

    def build_example(self, type: str, applies_to: List[str], xml_field: str, examples: List[str]) -> str:
        example = "<div class='codeblock'>"

        for node_type in applies_to:
            for value in examples:
                example += "&lt;" + node_type + " ... " + xml_field + "=<b>" + value + "</b> ... &gt;<br>"
            break
            # example += "<br>"

        example += "</div>"

        return example

    ############################################################################
    # Generate Auto Docs
    #
    # This will output documentation for a single category of attributes.
    ############################################################################
    def generate_auto_docs(self, metadata: Dict[str, SchemaType], content: Dict[str, str]) -> Tuple[str, List[FieldRow]]:
        file_loader = FileSystemLoader('web_templates')
        env = Environment(loader=file_loader)
        template = env.get_template("infotable.html")

        field_rows = []
        for fieldkey, fieldval in metadata.items():
            valid_values = ""

            if fieldval['type'] == "MultiflagValue" or fieldval['type'] == "Enum":

                if (fieldval["type"] == "MultiflagValue"):
                    pairings = fieldval["flags"]
                elif (fieldval["type"] == "Enum"):
                    pairings = fieldval["values"]
                else:
                    raise ValueError("Type was MultiflagValue or Enum but not MultiflagValue or Enum. Not sure what happened.")

                example = self.build_example(
                    type=fieldval['type'],
                    applies_to=fieldval['applies_to'],
                    xml_field=fieldval['xml_fields'][0],
                    examples=self.get_fixed_option_examples(
                        field_type=fieldval['type'],
                        pairings=pairings
                    )
                )

                valid_values = "<table class='flagbox'><tr><th>XML Value</th><td></td><th>"

                if (fieldval["type"] == "MultiflagValue"):
                    valid_values += "Set Flag"
                elif (fieldval["type"] == "Enum"):
                    valid_values += "Enum Value"
                valid_values += "</th></tr>"

                for elem in pairings:
                    valid_values += "<tr><td class='flag_valuebox'>"
                    for value in pairings[elem]:
                        valid_values += "\"" + value + "\"<br>"
                    valid_values += "</td><td class='flag_arrowbox'>&#10142;</td><td class='flag_namebox'>" + elem + "</td></tr>"
                valid_values += "</table>"

            elif fieldval['type'] == "CompoundValue":

                example = self.build_example(
                    type=fieldval['type'],
                    applies_to=fieldval['applies_to'],
                    xml_field=fieldval['xml_fields'][0],
                    examples=["???TODO???"]
                )
                # ",".join( [ self.get_examples(x['type'], fieldval['applies_to'], fieldval['xml_fields'][0]) for x in fieldval['components'] ])
            else:
                example = self.build_example(
                    type=fieldval['type'],
                    applies_to=fieldval['applies_to'],
                    xml_field=fieldval['xml_fields'][0],
                    examples=self.get_examples(
                        field_type=fieldval['type'],
                        # pairings=pairings
                    )
                )
                # self.get_examples(fieldval['type'], fieldval['applies_to'], fieldval['xml_fieldsval'][0])

            field_rows.append(FieldRow(
                name=fieldval["name"],
                xml_attribute=fieldval["xml_fields"][0],
                alternate_xml_attributes=fieldval["xml_fields"][1:],
                binary_field=fieldval["protobuf_field"],
                data_type=fieldval["type"],
                usable_on_html="<br>".join(fieldval["applies_to"]),
                example=example,
                valid_values_html=valid_values,
                is_sub_field=False,
                sub_fields=[],
                description=content[fieldkey]  # todo:
            ))

            if fieldval['type'] == "CompoundValue":
                for component_field in fieldval["components"]:
                    field_rows.append(FieldRow(
                        name=component_field["name"],
                        xml_attribute=component_field["xml_fields"][0],
                        alternate_xml_attributes=component_field["xml_fields"][1:],
                        binary_field=fieldval["protobuf_field"] + "." + component_field["protobuf_field"],
                        data_type=component_field["type"],
                        usable_on_html="<br>".join(fieldval["applies_to"]),
                        example=self.build_example(
                            type=component_field["type"],
                            applies_to=fieldval["applies_to"],
                            xml_field=fieldval["xml_fields"][0],
                            examples=["???TODO2???"],
                        ),
                        description=content[fieldkey],
                        valid_values_html=valid_values,
                        is_sub_field=True,
                        sub_fields=[]
                    ))

        return template.render(field_rows=field_rows), field_rows


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
# capitalize
#
# A helper function to take a snake case string and capitalize the first letter
# of each word in the string. A delimiter can be passed in to change the
# characters inserted between the newly capitalized words.
################################################################################
def capitalize(word: str, delimiter: str = " ") -> str:
    wordarray = word.split("_")
    capital_word_array = []

    for each_word in wordarray:
        capital_each_word = each_word.capitalize()
        capital_word_array.append(capital_each_word)

    return delimiter.join(capital_word_array)


################################################################################
# lowercase
#
# A helper function to take each word in the string and convert it to a snake
# case string of lowercase letters. A delimiter can be passed in to change the
# characters inserted between the newly lowercased words.
################################################################################
def lowercase(word: str, delimiter: str = "_") -> str:
    wordarray = word.split(" ")
    lower_word_array = []

    for each_word in wordarray:
        lower_each_word = each_word.lower()
        lower_word_array.append(lower_each_word)

    return delimiter.join(lower_word_array)


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


################################################################################
# normalize
#
# A helper function to take a string and convert it to a string of
# lowercase letters.
################################################################################
def normalize(word: str) -> str:
    normalized_word_array = []

    for i, letter in enumerate(word):
        if letter.isupper():
            normalized_word_array.append(letter.lower())
        else:
            normalized_word_array.append(letter)

    return "".join(normalized_word_array)


################################################################################
# main
#
# The first function that gets called. Is in change of parsing the input
# markdown files, and then creating the desired output files.
################################################################################
def main() -> None:
    generator = Generator()
    markdown_doc_directory = "../doc"

    for directory in os.listdir(markdown_doc_directory):
        full_markdown_doc_directory = os.path.join(markdown_doc_directory, directory)
        if os.path.isdir(full_markdown_doc_directory):
            generator.load_input_doc(full_markdown_doc_directory)

    generator.delete_generated_docs("../web_docs")
    generator.delete_generated_docs("../src/")
    generator.delete_generated_docs("../src/attribute")
    generator.write_webdocs("../web_docs/")
    generator.write_cpp_classes("../src/")
    generator.write_attribute("../src/attribute")


main()
