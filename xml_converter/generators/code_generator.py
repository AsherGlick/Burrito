from jsonschema import validate  # type:ignore
from jsonschema.exceptions import ValidationError  # type:ignore
import yaml
import frontmatter  # type:ignore
from typing import Any, Dict, List, Tuple
import os
import markdown
from dataclasses import dataclass
from jinja2 import Template, FileSystemLoader, Environment


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
    metadata: Dict[str, Any]
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

class Generator:
    data: Dict[str, Document] = {}


    def load_input_doc(self, dir_path: str) -> None:
        for filepath in os.listdir(dir_path):
            filepath = os.path.join(dir_path,filepath)
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
    
    def write_parseable(self, output_directory: str) -> None:
        file_loader = FileSystemLoader('cpp_templates')
        env = Environment(loader=file_loader)
        template = env.get_template("parseabletemplate.hpp")
        attributenames: Dict[str,str] = {}
        
        for filepath in self.data.keys():
            filename = os.path.basename(filepath)
            attributenames[filepath]= filename.split(".md")[0]
     
        pages = ["Category","Icon","Trail"]
        
        for page in pages:
            content: Dict[str,str] = {}
            metadata: Dict[str,List[str]] = {}
            
            for attributename in attributenames:
                metadata[attributename] = self.data[attributename].metadata
            
            field_rows = self.generate_node_types(metadata, attributenames, page)

            with open(os.path.join(output_directory, page.lower() + ".hpp"), 'w') as f:

                f.write(template.render(
                            page=page,
                            fieldrows=sorted(field_rows),
                        ))

    def write_webdocs(self, output_directory: str) -> None:
        print("Writing output documentation")
        os.makedirs(output_directory, exist_ok=True)

        file_loader = FileSystemLoader('web_templates')
        env = Environment(loader=file_loader)
        template = env.get_template("documentation.html")
        categories: Dict[str,List[str]] = {}

        for filename in self.data.keys():
            category = os.path.basename(os.path.dirname(filename))
            if category not in categories:
                categories[category] = []
            categories[category].append(filename)       

        navigation_links = [ (capitalize(category), category) for category in sorted(categories.keys()) ]
        
        

        complete_field_row_list = []
        pages: Dict[str,List[str]] = {}

        for page in sorted(categories):
            content: Dict[str,str] = {}
            metadata: Dict[str,List[str]] = {}
            # Resets the content and metadata to empty for each loop

            for subpage in categories[page]:
                
                content[subpage] = markdown.markdown(self.data[subpage].content)
               
                metadata[subpage] = self.data[subpage].metadata

            generated_doc, field_rows = self.generate_auto_docs(metadata,content)

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


    # ###########################################################################
    # Generate Node Types
    
    # This will output code for a single category of nodes.
    # ###########################################################################
    
    def generate_node_types(self, metadata: Any, attributenames: Dict[str,str],page: str) -> List[str]:
        
        field_rows = []

        typechange: Dict[str,str] = {
            "Fixed32": "int",
            "Int32": "int",
            "Boolean": "bool",
            "Float32": "float",
            "String": "string",
        }
        
        for fieldkey,field in metadata.items():
            for x in attributenames:
                if fieldkey in x :
                    attributename = attributenames[x]
                   
            if page in field['applies_to']:
                if field['type'] in typechange:
                    newtype = typechange[field['type']]
                elif field['type'] == "Custom":
                    newtype = field['class']
                elif field['type'] in ["Enum","MultiflagValue","CompoundValue"]:
                    newtype = capitalize(attributename,delimiter="")
                    
                else :
                    raise ValueError("Unexpected type {field_type} for attribute {attributename}".format(
                        field_type=field['type'],
                        attributename=attributename,
                    ) )
               
                field_rows.append((attributename,newtype))
    
        return field_rows

    ############################################################################
    # Generate Auto Docs
    #
    # This will output documentation for a single category of attributes.
    ############################################################################
    def generate_auto_docs(self, metadata: Any,content: Dict[str,str]) -> Tuple[str, List[FieldRow]]:
        file_loader = FileSystemLoader('web_templates')
        env = Environment(loader=file_loader)
        template = env.get_template("infotable.html")


        field_rows = []
        for fieldkey,field in metadata.items():
      
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
                supported_by_html="<br>".join(field["compatability"]),
                usable_on_html="<br>".join(field["applies_to"]),
                example=example,
                valid_values_html=valid_values,
                is_sub_field=False,
                sub_fields=[], 
                description=content[fieldkey]# todo:
            ))

            if field['type'] == "CompoundValue":
                for component_field in field["components"]:
                    field_rows.append(FieldRow(
                        name=component_field["name"],
                        xml_attribute= component_field["xml_fields"][0],
                        alternate_xml_attributes=component_field["xml_fields"][1:],
                        binary_field= field["protobuf_field"] + "." + component_field["protobuf_field"],
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

def capitalize(word: str,delimiter: str = " ") -> str:
  
    wordarray = word.split("_")
    capital_word_array = []

    for each_word in wordarray:
        capital_each_word = each_word.capitalize()
        capital_word_array.append(capital_each_word)

    return delimiter.join(capital_word_array)
   
def main() -> None:
    generator = Generator()
    base_directory = "../doc"

    for directory in os.listdir(base_directory):  
        if os.path.isdir(os.path.join(base_directory, directory)):
            generator.load_input_doc(os.path.join(base_directory, directory))

    generator.write_webdocs("../web_docs/")
    generator.write_parseable("../src/")

main()