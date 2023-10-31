from lark import Lark, Transformer
from lark.lexer import Token
from typing import List

################################################################################
# This module parses a proto definition file with the goal of identifying the
# proto field types based on the proto field name. It uses a lark grammar to
# parse the proto file. The grammar is a stringy definition which makes typing
# harder. Most type checks are ignored in this file, but hopefully we will be
# able to add them back in over time once we can figure them out.
################################################################################

parser = Lark(
    grammar="""?start: syntax_directive? package_directive? (declaration)*

        syntax_directive: "syntax" "=" "\\"proto3\\"" ";"
        package_directive: "package" dotted_identifier ";"

        declaration: message | enum

        message: "message" CNAME "{" message_body "}"
        message_body: (field | enum)*

        enum: "enum" CNAME "{" enum_body "}"
        enum_body: (enum_value)*

        enum_value: CNAME "=" SIGNED_INT ";"

        field: (repeated_type | type) CNAME "=" SIGNED_INT ";"

        repeated_type: "repeated" type

        type: CNAME

        dotted_identifier: CNAME ("." CNAME)*

        LINE_COMMENT: /\\/\\/[^\\n]*/

        %import common.CNAME
        %import common.WS
        %import common.SIGNED_INT
        %ignore WS
        %ignore LINE_COMMENT""",
    start='start',
    parser='lalr'
)


# Define transformer
class ProtoDictTransformer(Transformer):  # type: ignore
    def start(self, items: List):  # type: ignore
        messages = {}
        for item in items:
            if type(item) is dict:
                messages.update(item)
            elif item is None:
                pass
            else:
                print(item,)
        return messages

    def package_directive(self, items):  # type: ignore
        return {"__package__": items[0]}

    def dotted_identifier(self, items):  # type: ignore
        return items

    # Ignore the syntax directive
    def syntax_directive(self, items) -> None:  # type: ignore
        return None

    # Ignore enums
    def enum(self, items) -> None:  # type: ignore
        return None

    def declaration(self, items):  # type: ignore
        if len(items) == 0:
            return "ERROR"
        if len(items) > 1:
            print("Got more then one declaration, the grammar may be bugged", items)
        return items[0]

    def message(self, items):  # type: ignore
        name, body = items
        return {name: body}

    def message_body(self, items):  # type: ignore
        return {k: v for d in items for k, v in d.items()}

    def field(self, items):  # type: ignore
        # Check for repeated type
        if len(items) == 3:
            type_name, field_name, index = items
            return {field_name: type_name}
        print("unknown field syntax, the grammer may be bugged")
        return {}

    def CNAME(self, item: Token):  # type: ignore
        return item.value

    def type(self, items: List):  # type: ignore
        if len(items) == 0:
            return "ERROR"
        if len(items) > 1:
            print("Got more then one type, the grammar may be bugged", items)
        return items[0]

    # repeated_type has type tokens which get processed by type() into strings
    def repeated_type(self, items: List[str]) -> str:
        if len(items) == 0:
            return "ERROR"
        if len(items) > 1:
            print("Got more then one repeated type, the grammar may be bugged", items)
        return "REPEATED[" + items[0] + "]"


def proto_to_dict(proto_str):  # type: ignore
    tree = parser.parse(proto_str)
    transformer = ProtoDictTransformer()
    return transformer.transform(tree)


################################################################################
# Gets all of the field types of the proto.
################################################################################
with open("../proto/waypoint.proto") as f:
    proto_field_types = proto_to_dict(f.read())  # type: ignore


def get_proto_field_type(message: str, field: str) -> str:

    field_type = message
    multipart_field = field.split(".")

    for field in multipart_field:
        field_type = proto_field_types[field_type][field]

    return field_type
