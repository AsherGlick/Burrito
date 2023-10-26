from dataclasses import dataclass, field
from typing import List, Optional

@dataclass
class Testcase:
    name: str
    xml_input_paths: List[str] = field(default_factory=list)
    proto_input_paths: List[str] = field(default_factory=list)

    # TODO: Eventually the expected output paths wont be optional
    expected_output_xml_path: Optional[str] = None
    expected_output_proto_path: Optional[str] = None

    expected_stdout: List[str] = field(default_factory=list)
    expected_stderr: List[str] = field(default_factory=list)
    expected_returncode: int = 0


testcases: List[Testcase] = [
    Testcase(
        name="canfade_valid",
        xml_input_paths=["./inputs/xml_can_fade_valid"],
        expected_output_xml_path="./expected_outputs/xml_can_fade_valid",
    ),
    Testcase(
        name="canfade_invalid",
        xml_input_paths=["./inputs/xml_can_fade_invalid"],
        expected_output_xml_path="./expected_outputs/xml_can_fade_invalid",
        expected_stdout=[
            "Error: Found a boolean value that was not a '1', '0', 'true', or 'false'",
            "./inputs/xml_can_fade_invalid/xml_file.xml",
            '6 |<POI  CanFade="yes" Type="mycategory"/>',
            "  |               ^^^"
        ]
    )
]
