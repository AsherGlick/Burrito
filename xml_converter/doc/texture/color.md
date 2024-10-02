---
name: Color
type: CompoundCustomClass
class: Color
applies_to: [Icon, Trail]
xml_fields: [Color, BHColor]
protobuf_field: rgba_color
xml_bundled_components: ['Red', 'Green', 'Blue']
xml_separate_components: ['Alpha']
examples:
  - "FFFFFF"
  - "#FFFFFF"
  - "#FF8800"
  - "#CCCCCCFF"
  - "#000000CC"

components:
- name: Red
  type: Float32
  xml_fields: [Red]
  protobuf_field: null
  examples: ["0", "0.25", "0.5", "0.75", "1.0"]

- name: Green
  type: Float32
  xml_fields: [Green]
  protobuf_field: null
  examples: ["0", "0.25", "0.5", "0.75", "1.0"]

- name: Blue
  type: Float32
  xml_fields: [Blue]
  protobuf_field: null
  examples: ["0", "0.25", "0.5", "0.75", "1.0"]

- name: Alpha
  type: Float32
  xml_fields: [Alpha]
  protobuf_field: null
  examples: ["0", "0.25", "0.5", "0.75", "1.0"]


---
A multiplier color to tint the raw albedo texture of a marker of trail texture. (Unclear) Solid white will result in no change, solid black will result in a black texture.

Note: Burrito and BlishHUD treat the alpha channel of "Color" differently. Burrito treats the 7th and 8th hex of the color as a true alpha channel, just the same as if the Alpha attribute had been used. BlishHUD treats the 7th and 8th hex as a modulator for #FFFFFF.

A color of #00000000 will apear as transparent in Burrito, but will apear as the original texture on BlishHUD  
A color of #000000 will appear as black for both Burrito and BlishHUD  
A color of #FFFFFF00 will apear transparent for burrito and the original texture on BlishHUD  

Numbers are stored in an 8bit precision format so these are the floating point values that will be used if more precise values are specified: 0.000, 0.003, 0.007, 0.011, 0.015, 0.019, 0.023, 0.027, 0.031, 0.035, 0.039, 0.043, 0.047, 0.050, 0.054, 0.058, 0.062, 0.066, 0.070, 0.074, 0.078, 0.082, 0.086, 0.090, 0.094, 0.098, 0.101, 0.105, 0.109, 0.113, 0.117, 0.121, 0.125, 0.129, 0.133, 0.137, 0.141, 0.145, 0.149, 0.152, 0.156, 0.160, 0.164, 0.168, 0.172, 0.176, 0.180, 0.184, 0.188, 0.192, 0.196, 0.200, 0.203, 0.207, 0.211, 0.215, 0.219, 0.223, 0.227, 0.231, 0.235, 0.239, 0.243, 0.247, 0.250, 0.254, 0.258, 0.262, 0.266, 0.270, 0.274, 0.278, 0.282, 0.286, 0.290, 0.294, 0.298, 0.301, 0.305, 0.309, 0.313, 0.317, 0.321, 0.325, 0.329, 0.333, 0.337, 0.341, 0.345, 0.349, 0.352, 0.356, 0.360, 0.364, 0.368, 0.372, 0.376, 0.380, 0.384, 0.388, 0.392, 0.396, 0.400, 0.403, 0.407, 0.411, 0.415, 0.419, 0.423, 0.427, 0.431, 0.435, 0.439, 0.443, 0.447, 0.450, 0.454, 0.458, 0.462, 0.466, 0.470, 0.474, 0.478, 0.482, 0.486, 0.490, 0.494, 0.498, 0.501, 0.505, 0.509, 0.513, 0.517, 0.521, 0.525, 0.529, 0.533, 0.537, 0.541, 0.545, 0.549, 0.552, 0.556, 0.560, 0.564, 0.568, 0.572, 0.576, 0.580, 0.584, 0.588, 0.592, 0.596, 0.600, 0.603, 0.607, 0.611, 0.615, 0.619, 0.623, 0.627, 0.631, 0.635, 0.639, 0.643, 0.647, 0.650, 0.654, 0.658, 0.662, 0.666, 0.670, 0.674, 0.678, 0.682, 0.686, 0.690, 0.694, 0.698, 0.701, 0.705, 0.709, 0.713, 0.717, 0.721, 0.725, 0.729, 0.733, 0.737, 0.741, 0.745, 0.749, 0.752, 0.756, 0.760, 0.764, 0.768, 0.772, 0.776, 0.780, 0.784, 0.788, 0.792, 0.796, 0.800, 0.803, 0.807, 0.811, 0.815, 0.819, 0.823, 0.827, 0.831, 0.835, 0.839, 0.843, 0.847, 0.850, 0.854, 0.858, 0.862, 0.866, 0.870, 0.874, 0.878, 0.882, 0.886, 0.890, 0.894, 0.898, 0.901, 0.905, 0.909, 0.913, 0.917, 0.921, 0.925, 0.929, 0.933, 0.937, 0.941, 0.945, 0.949, 0.952, 0.956, 0.960, 0.964, 0.968, 0.972, 0.976, 0.980, 0.984, 0.988, 0.992, 0.996, 1.000

Values below 0 or above 1 will be clamped to 0 or 1

Notes
=====
https://blishhud.com/docs/markers/attributes/texture
https://blishhud.com/docs/markers/attributes/iconfile
https://blishhud.com/docs/markers/attributes/color
https://blishhud.com/docs/markers/attributes/animspeed

