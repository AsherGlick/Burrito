from jsonschema import validate
from jsonschema.exceptions import ValidationError
import yaml
import frontmatter
from typing import Any, Dict, List, Tuple
import os
import markdown
from dataclasses import dataclass
from jinja2 import Template, FileSystemLoader, Environment


schema = """
type: object
additionalProperties : false
required:
    - attribute_category
    - fields
properties:
    attribute_category:
        type: string
    fields:
        type: array
        items:
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
                                required: [name, type, xml_fields, protobuf_field, description, compatability]
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
                                    description:
                                        type: string
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
                        description:
                            type: string
                                """,
    shared_fields="type, name, applies_to, compatability, xml_fields, protobuf_field, description"
)




def validate_front_matter_schema(front_matter: Any) -> str:
    try:
        validate(front_matter, yaml.safe_load(schema))
    except ValidationError as e:
        return "Error Message: {} (Path: {}".format(e.message, e.json_path)
    return ""

@dataclass
class Document:
    metadata: Any
    content: str

@dataclass
class FieldRow:
    name: str
    xml_attribute: str
    alternate_xml_attributes: List[str]
    binary_field: str
    data_type: str
    description: str
    supported_by_html: str
    usable_on_html: str
    example: str
    valid_values_html: str
    is_sub_field: bool
    sub_fields: List["FieldRow"]

class Generator:
    data: Dict[str, Document] = {}


    def load_input_doc(self, filepath: str) -> None:
        try:
            document = frontmatter.load(filepath)
        except Exception as e:
            print(filepath)
            raise e

        error = validate_front_matter_schema(document.metadata)
        if error != "":
            print(filepath)
            print(error)

        filename = os.path.splitext(os.path.basename(filepath))[0]

        self.data[filename] = Document(
            metadata=document.metadata,
            content=document.content
        )
    
    def write_webdocs(self, output_directory: str) -> None:
        print("Writing output documentation")
        os.makedirs(output_directory, exist_ok=True)

        file_loader = FileSystemLoader('web_templates')
        env = Environment(loader=file_loader)
        template = env.get_template("documentation.html")

        navigation_links = [ (self.data[x].metadata['attribute_category'], x) for x in sorted(self.data.keys()) ]

        complete_field_row_list = []

        for page in sorted(self.data.keys()):
            content = self.data[page].content
            metadata = self.data[page].metadata

            generated_doc, field_rows = self.generate_auto_docs(metadata)

            for field_row in field_rows:
                complete_field_row_list.append(field_row)

            html = markdown.markdown(content)

            with open(os.path.join(output_directory, page + ".html"), 'w') as f:

                f.write(template.render(
                    generated_doc=generated_doc,
                    content=html,
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
        examples.append("\"" + solo_field +"\"")
        if len(combo_field) > 0:
            examples.append("\"" + ",".join(combo_field) +"\"")

        return examples

    def build_example(self, type: str, applies_to: List[str], xml_field: str, examples: List[str]) -> str:
        example = "<div class='codeblock'>"

        for node_type in applies_to:
            for value in examples:
                example += "&lt;" + node_type + " ... " + xml_field + "=<b>" + value +"</b> ... &gt;<br>"
            break
            # example += "<br>"
        


        example +="</div>"

        return example


    ############################################################################
    # Generate Auto Docs
    #
    # This will output documentation for a single category of attributes.
    ############################################################################
    def generate_auto_docs(self, metadata: Any) -> Tuple[str, List[FieldRow]]:
        file_loader = FileSystemLoader('web_templates')
        env = Environment(loader=file_loader)
        template = env.get_template("infotable.html")


        field_rows = []
        for field in metadata["fields"]:
            
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
                xml_attribute = field["xml_fields"][0],
                alternate_xml_attributes=field["xml_fields"][1:],
                binary_field=field["protobuf_field"],
                data_type=field["type"],
                description=field["description"],
                supported_by_html="<br>".join(field["compatability"]),
                usable_on_html="<br>".join(field["applies_to"]),
                example=example,
                valid_values_html=valid_values,
                is_sub_field=False,
                sub_fields=[], # todo:
            ))

            if field['type'] == "CompoundValue":
                for component_field in field["components"]:
                    field_rows.append(FieldRow(
                        name=component_field["name"],
                        xml_attribute= component_field["xml_fields"][0],
                        alternate_xml_attributes=component_field["xml_fields"][1:],
                        binary_field= field["protobuf_field"] + "." + component_field["protobuf_field"],
                        data_type=component_field["type"],
                        description=component_field["description"],
                        supported_by_html="<br>".join(component_field["compatability"]),
                        usable_on_html="<br>".join(field["applies_to"]),
                        example=self.build_example(
                            type=component_field["type"],
                            applies_to=field["applies_to"],
                            xml_field=field["xml_fields"][0],
                            examples=["???TODO2???"]
                        ),
                        valid_values_html=valid_values,
                        is_sub_field=True,
                        sub_fields=[]

                    ))

        return template.render(field_rows=field_rows), field_rows


def main() -> None:
    generator = Generator()
    base_directory = "../doc"

    for filename in os.listdir(base_directory):
        if filename.endswith('.md'):
            generator.load_input_doc(os.path.join(base_directory, filename))


    generator.write_webdocs("../web_docs/")

main()