from jsonschema import validate  # type:ignore
from jsonschema.exceptions import ValidationError  # type:ignore
import yaml
import frontmatter  # type:ignore
from typing import Any, Dict, List, Tuple, Set
import os
import markdown
from dataclasses import dataclass
from jinja2 import Template, FileSystemLoader, Environment

SchemaType = Dict[str, Any]
schema = """
type: object
properties:
    type:
        type: string
        enum: [Int32, Fixed32, Float32, String, Boolean, MultiflagValue, Enum, CompoundValue, Custom]
allOf:
    #############################
    # Int32 Type
    #############################
    - if:
        properties:
            type:
                const: Int32
      then:
        additionalProperties: false
        required: [{shared_fields}]
        properties:
            {shared_field_properties}

    #############################
    # Fixed32 Type
    #############################
    - if:
        properties:
            type:
                const: Fixed32
      then:
        additionalProperties: false
        required: [{shared_fields}]
        properties:
            {shared_field_properties}

    #############################
    # Float32 Type
    #############################
    - if:
        properties:
            type:
                const: Float32
      then:
        additionalProperties: false
        required: [{shared_fields}]
        properties:
            {shared_field_properties}

    #############################
    # String Type
    #############################
    - if:
        properties:
            type:
                const: String
      then:
        additionalProperties: false
        required: [{shared_fields}]
        properties:
            {shared_field_properties}

    #############################
    # Boolean Type
    #############################
    - if:
        properties:
            type:
                const: Boolean
      then:
        additionalProperties: false
        required: [{shared_fields}]
        properties:
            {shared_field_properties}

    #############################
    # MultiflagValue Type
    #############################
    - if:
        properties:
            type:
                const: MultiflagValue
      then:
        additionalProperties: false
        required: [{shared_fields}, flags]
        properties:
            {shared_field_properties}
            flags:
                type: object
                patternProperties:
                    "^[a-z_]+$":
                        type: array
                        items:
                            type: string

    #############################
    # Enum Type
    #############################
    - if:
        properties:
            type:
                const: Enum
      then:
        additionalProperties: false
        required: [{shared_fields}, values]
        properties:
            {shared_field_properties}
            values:
                type: object
                patternProperties:
                    "^[a-z_]+$":
                        type: array
                        items:
                            type: string

    #############################
    # CompoundValue Type
    #############################
    - if:
        properties:
            type:
                const: CompoundValue
      then:
        additionalProperties: false
        required: [{shared_fields}, xml_export, components]
        properties:
            {shared_field_properties}
            xml_export:
                type: string
                enum:
                    - Parent Only
                    - Parent and Children
                    - Children Only
            components:
                type: array
                items:
                    type: object
                    additionalProperties: false
                    required: [name, type, xml_fields, protobuf_field, compatability]
                    properties:
                        name:
                            type: string
                        type:
                            type: string
                            enum: [Int32, Fixed32, Float32]
                        xml_fields:
                            type: array
                            items:
                                type: string
                                pattern: "^[A-Za-z]+$"
                        protobuf_field:
                            type: string
                            pattern: "^[a-z_.]+$"
                        compatability:
                            type: array
                            items:
                                type: string
                                enum: [BlishHUD, Burrito, TacO]


    #############################
    # Custom Type
    #############################
    - if:
        properties:
            type:
                const: Custom
      then:
        additionalProperties: false
        required: [{shared_fields}, class]
        properties:
            {shared_field_properties}
            class:
                type: string
            side_effects:
                type: array
                items:
                    type: string

""".format(
    shared_field_properties="""type:
                type: string
            name:
                type: string
            applies_to:
                type: array
                items:
                    type: string
                    enum: [Icon, Trail, Category]
            compatability:
                type: array
                items:
                    type: string
                    enum: [BlishHUD, Burrito, TacO]
            xml_fields:
                type: array
                items:
                    type: string
                    pattern: "^[A-Za-z]+$"
            protobuf_field:
                type: string
                pattern: "^[a-z_.]+$"
    """,
    shared_fields="type, name, applies_to, compatability, xml_fields, protobuf_field"
)


def validate_front_matter_schema(front_matter: Any) -> str:
    try:
        validate(front_matter, yaml.safe_load(schema))
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
    supported_by_html: str
    usable_on_html: str
    example: str
    valid_values_html: str
    is_sub_field: bool
    sub_fields: List["FieldRow"]
    description: str


doc_type_to_cpp_type: Dict[str, str] = {
    "Fixed32": "int",
    "Int32": "int",
    "Boolean": "bool",
    "Float32": "float",
    "String": "string",
}


@dataclass
class AttributeVariable:
    attribute_name: str
    cpp_type: str
    class_name: str
    xml_fields: List[str]


def get_attribute_variable_key(attribute_variable: AttributeVariable) -> str:
    return attribute_variable.attribute_name


class Generator:
    data: Dict[str, Document] = {}

    def delete_generated_docs(self, dir_path: str) -> None:
        for filepath in os.listdir(dir_path):
            filepath = os.path.join(dir_path, filepath)
            if filepath.endswith("_gen.hpp") or filepath.endswith("_gen.cpp") or filepath.endswith("_gen.html"):
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
        env = Environment(loader=file_loader)
        attribute_names: Dict[str, str] = {}
        header_template: Template = env.get_template("class_template.hpp")
        code_template: Template = env.get_template("class_template.cpp")
        attributes_of_type_marker_category: List[str] = []
        cpp_classes: Dict[str, str] = {
            "Category": "MarkerCategory",
            "Icon": "POI",
            "Trail": "Trail"
        }

        for filepath in self.data.keys():
            filename = os.path.basename(filepath)
            attribute_names[filepath] = filename.split(".md")[0]

        for cpp_class in cpp_classes:
            metadata: Dict[str, SchemaType] = {}
            attributes_of_type_marker_category = []

            for attribute_name in attribute_names:
                metadata[attribute_name] = self.data[attribute_name].metadata

            attribute_variables: List[AttributeVariable]
            attribute_variables, cpp_include_paths = self.generate_cpp_variable_data(metadata, cpp_class, attribute_names)

            for attribute_variable in attribute_variables:
                if attribute_variable.class_name == "marker_category":
                    attributes_of_type_marker_category.append(attribute_variable.attribute_name)

            with open(os.path.join(output_directory, lowercase(cpp_class) + "_gen.hpp"), 'w') as f:
                f.write(header_template.render(
                    cpp_class=cpp_class,
                    attribute_variables=sorted(attribute_variables, key=get_attribute_variable_key),
                    cpp_include_paths=sorted(cpp_include_paths),
                    attributes_of_type_marker_category=attributes_of_type_marker_category,
                ))

            with open(os.path.join(output_directory, lowercase(cpp_class) + "_gen.cpp"), 'w') as f:
                f.write(code_template.render(
                    cpp_class=cpp_class,
                    cpp_class_header=lowercase(cpp_class),
                    xml_class_name=cpp_classes[cpp_class],
                    attribute_variables=sorted(attribute_variables, key=get_attribute_variable_key),
                    enumerate=enumerate,
                    attributes_of_type_marker_category=attributes_of_type_marker_category,
                ))

    ############################################################################
    # generate_cpp_variable_data
    #
    # This will return a list of tuples containing a tuple of the variables
    # needed for the templates, and a set of all of the dependencies
    # those variables will need to have included.
    ############################################################################
    def generate_cpp_variable_data(
        self,
        metadata: Dict[str, SchemaType],
        doc_type: str,
        attribute_names: Dict[str, str] = {}
    ) -> Tuple[List[AttributeVariable], Set[str]]:

        cpp_include_paths: Set[str] = set()
        attribute_name: str = ""
        attribute_variables: List[AttributeVariable] = []
        xml_fields: List[str] = []

        for fieldkey, field in metadata.items():
            for x in attribute_names:
                if fieldkey in x:
                    attribute_name = attribute_names[x]

            if doc_type in field['applies_to']:
                xml_fields = []
                if field['type'] in doc_type_to_cpp_type:
                    cpp_type = doc_type_to_cpp_type[field['type']]
                    class_name = cpp_type
                    cpp_include_paths.add(class_name)
                elif field['type'] == "Custom":
                    cpp_type = field['class']
                    class_name = insert_delimiter(field['class'], delimiter="_")
                    cpp_include_paths.add(class_name)
                elif field['type'] in ["Enum", "MultiflagValue", "CompoundValue"]:
                    cpp_type = capitalize(attribute_name, delimiter="")
                    class_name = attribute_name
                    cpp_include_paths.add(class_name + "_gen")
                else:
                    raise ValueError("Unexpected type {field_type} for attribute {attribute_name}".format(
                        field_type=field['type'],
                        attribute_name=attribute_name,
                    ))

                for item in field['xml_fields']:
                    xml_fields.append(lowercase(item, delimiter=""))

                # Compound Values are unique in that the components have xml fields in addition to the compound variable
                if field['type'] == "CompoundValue":
                    for component in field['components']:
                        for item in component['xml_fields']:
                            xml_fields.append(lowercase(item, delimiter=""))

                attribute_variable: AttributeVariable = AttributeVariable(
                    attribute_name=attribute_name,
                    cpp_type=cpp_type,
                    class_name=class_name,
                    xml_fields=xml_fields,
                )

                attribute_variables.append(attribute_variable)

        return attribute_variables, cpp_include_paths

    ############################################################################
    # write_attributes
    #
    # Creates the attribute files for attributes that contain multiple values
    ############################################################################
    def write_attribute(self, output_directory: str) -> None:
        print("Writing attributes")
        os.makedirs(output_directory, exist_ok=True)

        file_loader = FileSystemLoader('cpp_templates')
        env = Environment(loader=file_loader)
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
            attribute_name = attribute_names[filepath]
            metadata[filepath] = self.data[filepath].metadata

            if metadata[filepath]['type'] == "MultiflagValue":
                for flag in metadata[filepath]['flags']:
                    xml_fields = []
                    for item in metadata[filepath]['flags'][flag]:
                        xml_fields.append(normalize(item))

                    attribute_variable = AttributeVariable(
                        attribute_name=flag,
                        cpp_type="bool",
                        class_name=attribute_name,
                        xml_fields=xml_fields,
                    )
                    attribute_variables.append(attribute_variable)

            elif metadata[filepath]['type'] == "CompoundValue":
                for component in metadata[filepath]['components']:
                    xml_fields = []
                    if component['type'] not in doc_type_to_cpp_type:
                        raise ValueError("Unexpected type for component. Look at markdown file {attribute_name}".format(
                            attribute_name=attribute_name
                        ))
                    for item in component['xml_fields']:
                        xml_fields.append(normalize(item))
                    attribute_variable = AttributeVariable(
                        attribute_name=lowercase(component['name'], delimiter="_"),
                        cpp_type=doc_type_to_cpp_type[component['type']],
                        class_name=attribute_name,
                        xml_fields=xml_fields,
                    )
                    attribute_variables.append(attribute_variable)

            elif metadata[filepath]['type'] == "Enum":
                for value in metadata[filepath]['values']:
                    xml_fields = []
                    for item in metadata[filepath]['values'][value]:
                        xml_fields.append(normalize(item))
                    attribute_variable = AttributeVariable(
                        attribute_name=value,
                        cpp_type="str",
                        class_name=attribute_name,
                        xml_fields=xml_fields
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

            with open(os.path.join(output_directory, page + "_gen.html"), 'w') as f:

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
        for fieldkey, field in metadata.items():
            valid_values = ""

            if field['type'] == "MultiflagValue" or field['type'] == "Enum":

                if (field["type"] == "MultiflagValue"):
                    pairings = field["flags"]
                elif (field["type"] == "Enum"):
                    pairings = field["values"]
                else:
                    raise ValueError("Type was MultiflagValue or Enum but not MultiflagValue or Enum. Not sure what happened.")

                example = self.build_example(
                    type=field['type'],
                    applies_to=field['applies_to'],
                    xml_field=field['xml_fields'][0],
                    examples=self.get_fixed_option_examples(
                        field_type=field['type'],
                        pairings=pairings
                    )
                )

                valid_values = "<table class='flagbox'><tr><th>XML Value</th><td></td><th>"

                if (field["type"] == "MultiflagValue"):
                    valid_values += "Set Flag"
                elif (field["type"] == "Enum"):
                    valid_values += "Enum Value"
                valid_values += "</th></tr>"

                for elem in pairings:
                    valid_values += "<tr><td class='flag_valuebox'>"
                    for value in pairings[elem]:
                        valid_values += "\"" + value + "\"<br>"
                    valid_values += "</td><td class='flag_arrowbox'>&#10142;</td><td class='flag_namebox'>" + elem + "</td></tr>"
                valid_values += "</table>"

            elif field['type'] == "CompoundValue":

                example = self.build_example(
                    type=field['type'],
                    applies_to=field['applies_to'],
                    xml_field=field['xml_fields'][0],
                    examples=["???TODO???"]
                )
                # ",".join( [ self.get_examples(x['type'], field['applies_to'], field['xml_fields'][0]) for x in field['components'] ])
            else:
                example = self.build_example(
                    type=field['type'],
                    applies_to=field['applies_to'],
                    xml_field=field['xml_fields'][0],
                    examples=self.get_examples(
                        field_type=field['type'],
                        # pairings=pairings
                    )
                )
                # self.get_examples(field['type'], field['applies_to'], field['xml_fields'][0])

            field_rows.append(FieldRow(
                name=field["name"],
                xml_attribute=field["xml_fields"][0],
                alternate_xml_attributes=field["xml_fields"][1:],
                binary_field=field["protobuf_field"],
                data_type=field["type"],
                supported_by_html="<br>".join(field["compatability"]),
                usable_on_html="<br>".join(field["applies_to"]),
                example=example,
                valid_values_html=valid_values,
                is_sub_field=False,
                sub_fields=[],
                description=content[fieldkey]  # todo:
            ))

            if field['type'] == "CompoundValue":
                for component_field in field["components"]:
                    field_rows.append(FieldRow(
                        name=component_field["name"],
                        xml_attribute=component_field["xml_fields"][0],
                        alternate_xml_attributes=component_field["xml_fields"][1:],
                        binary_field=field["protobuf_field"] + "." + component_field["protobuf_field"],
                        data_type=component_field["type"],
                        supported_by_html="<br>".join(component_field["compatability"]),
                        usable_on_html="<br>".join(field["applies_to"]),
                        example=self.build_example(
                            type=component_field["type"],
                            applies_to=field["applies_to"],
                            xml_field=field["xml_fields"][0],
                            examples=["???TODO2???"],
                        ),
                        description=content[fieldkey],
                        valid_values_html=valid_values,
                        is_sub_field=True,
                        sub_fields=[]
                    ))

        return template.render(field_rows=field_rows), field_rows


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
def normalize(word: str) ->str:
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
