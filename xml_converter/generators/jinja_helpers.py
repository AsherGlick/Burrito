from dataclasses import dataclass
from jinja2.ext import Extension
from typing import Tuple, Optional, List
import enum
import re


################################################################################
# IndentFlow
#
# An enum to represent the different actions that can be taken for a given line
# of the jinja template.
################################################################################
class IndentFlow(enum.Enum):
    push = enum.auto()
    keep = enum.auto()
    pop = enum.auto()
    poppush = enum.auto()


################################################################################
# IndentStackElem
#
# A dataclass that keeps track of the indentation of a specific block of the
# template and can apply the indentation to all the lines in that block easily.
################################################################################
@dataclass
class IndentStackElem():
    indentation: str
    token: str
    startline: str
    lines: List[str]

    def apply_indent(self) -> List[str]:

        indented_lines = [self.indentation + self.startline]

        for line in unindent_block(self.lines):
            if line == "":
                indented_lines.append(line)
                continue
            indented_lines.append(self.indentation + line)

        return indented_lines


tag_regex = re.compile(r".*\{\%-?(.*)\-?%\}.*")
indentation_regex = re.compile(r"^(?P<indent>[ \t]*)(?P<line>.*)")


################################################################################
# unindent_blocks
#
# Attempts to read a jinja format string and remove extra indentation used only
# in the jinja templates due to things like {% for %} or {% if %} blocks.
################################################################################
class UnindentBlocks(Extension):
    def preprocess(self, source: str, name: Optional[str], filename: Optional[str] = None) -> str:

        indentation_stack: List[IndentStackElem] = [IndentStackElem("", "", "", [])]

        output_lines: List[str] = []

        for line in source.split("\n"):

            indentation_match = re.match(indentation_regex, line)
            if indentation_match is None:
                raise ValueError("Cannot Identify Indentation")

            whitespace = indentation_match.groupdict()["indent"]
            unindented_line = indentation_match.groupdict()["line"]

            flow = self.indent_flow(unindented_line)

            if flow[0] == IndentFlow.push:
                indentation_stack.append(IndentStackElem(whitespace, flow[1], unindented_line, []))

            elif flow[0] == IndentFlow.keep:
                # if we are not in a block just do the normal thing
                if len(indentation_stack) == 0:
                    output_lines.append(whitespace + unindented_line)
                else:
                    # TODO check flow[1]
                    indentation_stack[-1].lines.append(line)

            elif flow[0] == IndentFlow.poppush:
                chunk = indentation_stack.pop()
                indentation_stack[-1].lines += chunk.apply_indent()
                indentation_stack.append(IndentStackElem(
                    chunk.indentation,
                    chunk.token,
                    unindented_line,
                    []
                ))

            elif flow[0] == IndentFlow.pop:
                # TODO check flow[1]
                chunk = indentation_stack.pop()
                indentation_stack[-1].lines += chunk.apply_indent()
                indentation_stack[-1].lines.append(chunk.indentation + unindented_line)

        # TODO: sanitycheck that indentation stack has only 1 element
        return "\n".join(indentation_stack[0].lines)

    ############################################################################
    # indent_flow
    #
    # Returns how the indenting should change for subsequent lines
    #
    # First argument is push, keep, or pop to indicate what to do with the indentation stack
    # the second argument is the control block, for push that block should be added
    # for keep or pop, that value should be checked, or checked and removed
    #
    # if keep and the value is None then dont check and keep going.
    # The second value is mostly for sanity checking that all these blocks have pairs.
    # There will definitely be issues with this if there is a block that starts on a line by itself
    # but does not end on a line by itself
    ############################################################################
    def indent_flow(self, line: str) -> Tuple[IndentFlow, str]:
        matched_tags = parse_out_tags(line)

        if len(matched_tags) == 0:
            return IndentFlow.keep, ""

        if len(matched_tags) > 1:
            # This is maybe wrong, but we will assume for now that if there are
            # multiple tags on a single line then the entire "block" of those
            # tags is on the line and no indentation stuff should be changed.
            return IndentFlow.keep, ""

        parsed_tag_line = matched_tags[0].strip('- ')

        if parsed_tag_line.startswith("for"):
            return IndentFlow.push, "for"
        elif parsed_tag_line.startswith("endfor"):
            return IndentFlow.pop, "for"
        elif parsed_tag_line.startswith("if"):
            return IndentFlow.push, "if"
        elif parsed_tag_line.startswith("elif"):
            return IndentFlow.poppush, "if"
        elif parsed_tag_line.startswith("else"):
            return IndentFlow.poppush, "if"
        elif parsed_tag_line.startswith("endif"):
            return IndentFlow.pop, "if"

        # # TODO: Make tests for macro, call, filter
        # elif parsed_tag_line.startswith("macro"):
        #     return IndentFlow.push, "macro"
        # elif parsed_tag_line.startswith("endmacro"):
        #     return IndentFlow.pop, "macro"
        # elif parsed_tag_line.startswith("call"):
        #     return IndentFlow.push, "call"
        # elif parsed_tag_line.startswith("endcall"):
        #     return IndentFlow.pop, "call"
        # elif parsed_tag_line.startswith("filter"):
        #     return IndentFlow.push, "filter"
        # elif parsed_tag_line.startswith("endfilter"):
        #     return IndentFlow.pop, "filter"

        # # TODO: Do more testing to branch between set and blockset
        elif parsed_tag_line.startswith("set"):
            return IndentFlow.keep, ""
        #     return IndentFlow.push, "set" # or maybe `IndentFlow.keep, ""` if not a block set
        # elif parsed_tag_line.startswith("endset"):
        #     return IndentFlow.pop, "set"

        raise ValueError("Unknown Jinja2 Statement " + parsed_tag_line)


################################################################################
# unindent_block
#
# Takes a list of text lines and removes a uniform indent for every line.
# TODO: The organization of this function is pretty poor and should be redone.
# The best way to do this is probably by writing some tests first and then
# refactoring this function afterwards.
################################################################################
def unindent_block(block: List[str]) -> List[str]:
    common_indent = ""
    next_indent: Optional[str] = None
    index = -1

    searching = True
    while searching:
        index += 1

        for line in block:
            # Ignore blank lines so that they dont cause the removed indent to
            # be 0 characters long.
            if line == "":
                continue

            # Exit if we reach the end of a line
            if index >= len(line):
                searching = False
                break

            if next_indent is None:
                next_indent = line[index]

                # Stop searching when you get to non-whitespace
                if next_indent not in {" ", "\t"}:
                    searching = False
                    break

            # Exit if we find a non matching indent character in one of the other lines
            if next_indent != line[index]:
                searching = False
                break

        # A sanity check for if we never triggered anything in the for loop
        # EG: all empty lines
        if next_indent is None:
            break

        common_indent += next_indent
        next_indent = None

    return [line[index:] for line in block]


################################################################################
# parse_out_tags
#
# This function parses out jinja control flow tags from a string of text.
################################################################################
def parse_out_tags(line: str) -> List[str]:
    tags: List[str] = []

    while True:
        start_index = line.find("{%")

        if start_index == -1:
            return tags

        end_index = line.find("%}")

        if end_index == -1:
            return tags

        tags.append(line[start_index + 2:end_index])
        line = line[end_index + 2:]
