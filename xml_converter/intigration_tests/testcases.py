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
        expected_output_proto_path="./expected_outputs/proto_can_fade_valid",
    ),
    Testcase(
        name="canfade_invalid",
        xml_input_paths=["./inputs/xml_can_fade_invalid"],
        expected_output_xml_path="./expected_outputs/xml_can_fade_invalid",
        expected_output_proto_path="./expected_outputs/proto_can_fade_invalid",
        expected_stdout=[
            "Error: Found a boolean value that was not a '1', '0', 'true', or 'false'",
            "./inputs/xml_can_fade_invalid/xml_file.xml",
            '6 |        <POI CanFade="yes" Type="mycategory" XPos="169.81" YPos="210.65" ZPos="215.83" MapID="50" />',
            "  |                      ^^^",
            "Error: Found a boolean value that was not a '1', '0', 'true', or 'false'",
            './inputs/xml_can_fade_invalid/xml_file.xml',
            '7 |        <POI CanFade="はい" Type="mycategory" XPos="169.81" YPos="210.65" ZPos="215.83" MapID="50" />',
            '  |                      ^^^^^^',

        ]
    ),
    Testcase(
        name="mountfilter_valid",
        xml_input_paths=["./inputs/xml_mount_filter_valid"],
        expected_output_xml_path="./expected_outputs/xml_mount_filter_valid",
        expected_output_proto_path="./expected_outputs/proto_mount_filter_valid",
    ),
    Testcase(
        name="mountfilter_invalid",
        xml_input_paths=["./inputs/xml_mount_filter_invalid"],
        expected_output_xml_path="./expected_outputs/xml_mount_filter_invalid",
        expected_output_proto_path="./expected_outputs/proto_mount_filter_invalid",
        expected_stdout=[
            'Error: Invalid Filter for MountFilter. Found ',
            './inputs/xml_mount_filter_invalid/xml_file.xml',
            '6 |        <POI Mount="" Type="mycategory" XPos="169.81" YPos="210.65" ZPos="215.83" MapID="50" />',
            '  |                    ',
            'Error: Invalid Filter for MountFilter. Found NotAMount',
            './inputs/xml_mount_filter_invalid/xml_file.xml',
            '7 |        <POI Mount="NotAMount" Type="mycategory" XPos="169.81" YPos="210.65" ZPos="215.83" MapID="50" />',
            '  |                    ^^^^^^^^^',
            'Error: Invalid Filter for MountFilter. Found ',
            './inputs/xml_mount_filter_invalid/xml_file.xml',
            '8 |        <POI Mount="Raptor,Springer," Type="mycategory" XPos="169.81" YPos="210.65" ZPos="215.83" MapID="50" />',
            '  |                    ^^^^^^^^^^^^^^^^',
            'Error: Invalid Filter for MountFilter. Found NotAMount',
            './inputs/xml_mount_filter_invalid/xml_file.xml',
            '9 |        <POI Mount="Raptor,NotAMount,Springer" Type="mycategory" XPos="169.81" YPos="210.65" ZPos="215.83" MapID="50" />',
            '  |                    ^^^^^^^^^^^^^^^^^^^^^^^^^',
        ]
    ),

    Testcase(
        name="achievement_bitmask",
        xml_input_paths=["./inputs/xml_achievement_bitmask_valid"],
        expected_output_xml_path="./expected_outputs/xml_achievement_bitmask_valid",
        expected_output_proto_path="./expected_outputs/proto_achievement_bitmask_valid",
    ),
    Testcase(
        name="achievement_id",
        xml_input_paths=["./inputs/xml_achievement_id"],
        expected_output_xml_path="./expected_outputs/xml_achievement_id",
        expected_output_proto_path="./expected_outputs/proto_achievement_id",
    ),
    Testcase(
        name="is_wall",
        xml_input_paths=["./inputs/xml_is_wall"],
        expected_output_xml_path="./expected_outputs/xml_is_wall",
        expected_output_proto_path="./expected_outputs/proto_is_wall",
    ),
    Testcase(
        name="fade",
        xml_input_paths=["./inputs/xml_fade"],
        expected_output_xml_path="./expected_outputs/xml_fade",
        expected_output_proto_path="./expected_outputs/proto_fade",
    ),
    Testcase(
        name="festival_filter",
        xml_input_paths=["./inputs/xml_festival_filter"],
        expected_output_xml_path="./expected_outputs/xml_festival_filter",
        expected_output_proto_path="./expected_outputs/proto_festival_filter",
    ),
    Testcase(
        name="map_id",
        xml_input_paths=["./inputs/xml_map_id"],
        expected_output_xml_path="./expected_outputs/xml_map_id",
        expected_output_proto_path="./expected_outputs/proto_map_id",
    ),
    Testcase(        
        name="map_type_filter",
        xml_input_paths=["./inputs/xml_map_type_filter"],
        expected_output_xml_path="./expected_outputs/xml_map_type_filter",
        expected_output_proto_path="./expected_outputs/proto_map_type_filter",
    ),
    Testcase(
        name="profession_filter",
        xml_input_paths=["./inputs/xml_profession_filter"],
        expected_output_xml_path="./expected_outputs/xml_profession_filter",
        expected_output_proto_path="./expected_outputs/proto_profession_filter",
    ),
    Testcase(
        name="animation_speed",
        xml_input_paths=["./inputs/xml_animation_speed"],
        expected_output_xml_path="./expected_outputs/xml_animation_speed",
        expected_output_proto_path="./expected_outputs/proto_animation_speed"
    ),
    Testcase(
        name="cull_chirality",
        xml_input_paths=["./inputs/xml_cull_chirality"],
        expected_output_xml_path="./expected_outputs/xml_cull_chirality",
        expected_output_proto_path="./expected_outputs/proto_cull_chirality",
    ),
    Testcase(
        name="specialization_filter",
        xml_input_paths=["./inputs/xml_specialization_filter"],
        expected_output_xml_path="./expected_outputs/xml_specialization_filter",
        expected_output_proto_path="./expected_outputs/proto_specialization_filter",
    ),
    Testcase(
        name="species_filter",
        xml_input_paths=["./inputs/xml_species_filter"],
        expected_output_xml_path="./expected_outputs/xml_species_filter",
        expected_output_proto_path="./expected_outputs/proto_species_filter",
    ),
    Testcase(
        name="texture",
        xml_input_paths=["./inputs/xml_texture"],
        expected_output_xml_path="./expected_outputs/xml_texture",
        expected_output_proto_path="./expected_outputs/proto_texture",
    ),
]
