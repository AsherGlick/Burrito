---
name: Map Type Filter
type: MultiflagValue
applies_to: [Icon, Trail]
xml_fields: [MapType]
protobuf_field: map_type_filter
flags:
  # Unknown map type
  unknown_map: ["Unknown"]    
  # Redirect map type, e.g. when logging in while in a PvP match.
  redirect_map: [redirect]
  # Character create map type.
  character_create_map: [charactercreate]
  # PvP map type.
  pvp_map: [pvp]
  # GvG map type. Unused.
  gvg_map: [gvg]
  # Instance map type, e.g. dungeons and story content.
  instance_map: [instance]
  # Public map type, e.g. open world.
  public_map: [public]
  # Tournament map type. Probably unused.
  tournament_map: [tournament]
  # Tutorial map type.
  tutorial_map: [tutorial]
  # User tournament map type. Probably unused.
  user_tournament_map: [usertournament]
  # Eternal Battlegrounds (WvW) map type.
  center_map: [center]
  # Eternal Battlegrounds (WvW) map type.
  eternal_battlegrounds_map: [eternalbattlegrounds]
  # Blue Borderlands (WvW) map type.
  bluehome_map: [bluehome]
  # Blue Borderlands (WvW) map type.
  blue_borderlands_map: [blueborderlands]
  # Green Borderlands (WvW) map type.
  green_home_map: [greenhome]
  # Green Borderlands (WvW) map type.
  green_borderlands_map: [greenborderlands]
  # Red Borderlands (WvW) map type.
  red_home_map: [redhome]
  # Red Borderlands (WvW) map type.
  red_borderlands_map: [redborderlands]
  # Fortune's Vale. Unused.
  fortunes_vale_map: [fortunesvale]
  # Obsidian Sanctum (WvW) map type.
  jump_puzzle_map: [jumppuzzle]
  # Obsidian Sanctum (WvW) map type.
  obsidian_sanctum_map: [obsidiansanctum]
  # Edge of the Mists (WvW) map type.
  edge_of_the_mists_map: [edgeofthemists]
  # Mini public map type, e.g. Dry Top, the Silverwastes and Mistlock Sanctuary.
  public_mini_map: [publicmini]
  # WvW lounge map type, e.g. Armistice Bastion.
  wvw_lounge_map: [wvwlounge]
---
The types of maps this object should be displayed on.

Notes
=====
https://blishhud.com/docs/markers/attributes/maptype
