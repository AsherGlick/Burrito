from jsonschema import validate  # type:ignore
from jsonschema.exceptions import ValidationError  # type:ignore
import frontmatter  # type:ignore
from typing import Any, Dict, List, Tuple, Final, Set
import os
import markdown
from dataclasses import dataclass
from jinja2 import FileSystemLoader, Environment
from schema import string_t, array_t, enum_t, union_t, union_partial_t, pattern_dictionary_t, object_t, boolean_t, DefType
from protobuf_types import get_proto_field_type
from util import capitalize, SchemaType, Document
from generate_cpp import write_cpp_classes, write_attribute

XML_ATTRIBUTE_REGEX: Final[str] = "^[A-Za-z]+$"
PROTO_FIELD_REGEX: Final[str] = "^[a-z_.]+$"
INTERNAL_VARIABLE_REGEX: Final[str] = "^[a-z_]+$"
ATTRIBUTE_NAME_REGEX: Final[str] = "^[A-Za-z ]+$"
CUSTOM_SERIALIZER_REGEX: Final[str] = r"^(?:read|write)(?:\.xml|\.proto)(?:\.icon|\.trail)?$"

shared_field_properties: Dict[str, DefType] = {
    "type": string_t(),
    "name": string_t(pattern=ATTRIBUTE_NAME_REGEX),
    "applies_to": array_t(enum_t(["Icon", "Trail", "Category"])),
    "xml_fields": array_t(string_t(pattern=XML_ATTRIBUTE_REGEX)),
    "protobuf_field": string_t(pattern=PROTO_FIELD_REGEX),
}

custom_serial_functions: Dict[str, DefType] = {
    "custom_functions": pattern_dictionary_t({CUSTOM_SERIALIZER_REGEX: object_t({
        "function": string_t(),
        "side_effects": array_t(string_t()),
    })}),
}

schema = union_t({
    "Int32": union_partial_t(required=shared_field_properties, optional=custom_serial_functions),
    "Fixed32": union_partial_t(required=shared_field_properties, optional=custom_serial_functions),
    "Float32": union_partial_t(required=shared_field_properties, optional=custom_serial_functions),
    "String": union_partial_t(required=shared_field_properties, optional=custom_serial_functions),
    "Boolean": union_partial_t(required=shared_field_properties, optional=custom_serial_functions),
    "MultiflagValue": union_partial_t(
        required={**shared_field_properties, **{
            "flags": pattern_dictionary_t({INTERNAL_VARIABLE_REGEX: array_t(string_t())}),
        }},
        optional=custom_serial_functions,
    ),
    "Enum": union_partial_t(
        required={**shared_field_properties, **{
            "values": pattern_dictionary_t({INTERNAL_VARIABLE_REGEX: array_t(string_t())})
        }},
        optional=custom_serial_functions,
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
        }},
        optional=custom_serial_functions,
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
        }},
        optional=custom_serial_functions,
    ),
    "Custom": union_partial_t(
        required={**shared_field_properties, **{"class": string_t()}},
        optional={
            **custom_serial_functions,
            "uses_file_path": boolean_t(),  # This will eventually be part of a struct that is passed into everything
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
class FieldRow:
    name: str
    xml_attribute: str
    alternate_xml_attributes: List[str]
    binary_field: str
    binary_field_type: str
    data_type: str
    usable_on_html: str
    example: str
    valid_values_html: str
    is_sub_field: bool
    sub_fields: List["FieldRow"]
    description: str


class Generator:
    data: Dict[str, Document] = {}

    def delete_generated_docs(self, dir_path: str, skip: List[str] = []) -> None:
        excluded_files: Set[str] = set(skip)
        for filepath in os.listdir(dir_path):
            filepath = os.path.join(dir_path, filepath)
            if (filepath.endswith("_gen.hpp") or filepath.endswith("_gen.cpp") or filepath.endswith(".html")) and filepath not in excluded_files:
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
                content[subpage] = markdown.markdown(self.data[subpage].content, extensions=['extra', 'codehilite'])
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

            proto_field_type: str = ""
            for marker_type in fieldval["applies_to"]:
                proto_field_type = get_proto_field_type(marker_type, fieldval["protobuf_field"])
                # TODO: catch discrepencies if the proto field types across
                # different messages have differing types. This will be caught
                # in the cpp code regardless.

            field_rows.append(FieldRow(
                name=fieldval["name"],
                xml_attribute=fieldval["xml_fields"][0],
                alternate_xml_attributes=fieldval["xml_fields"][1:],
                binary_field=fieldval["protobuf_field"],
                binary_field_type=proto_field_type,
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

                    binary_field_name = fieldval["protobuf_field"] + "." + component_field["protobuf_field"]

                    component_field_type: str = ""
                    for marker_type in fieldval["applies_to"]:
                        component_field_type = get_proto_field_type(marker_type, binary_field_name)
                        # TODO: catch discrepencies if the proto field types across
                        # different messages have differing types. This will be caught
                        # in the cpp code regardless.

                    field_rows.append(FieldRow(
                        name=component_field["name"],
                        xml_attribute=component_field["xml_fields"][0],
                        alternate_xml_attributes=component_field["xml_fields"][1:],
                        binary_field=binary_field_name,
                        binary_field_type=component_field_type,
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
    generator.write_webdocs("../web_docs/")
    write_cpp_classes("../src/", generator.data)

    written_attributes = write_attribute("../src/attribute", generator.data)
    generator.delete_generated_docs("../src/attribute", skip=written_attributes)


main()
