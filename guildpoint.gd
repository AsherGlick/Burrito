const PROTO_VERSION = 3

#
# BSD 3-Clause License
#
# Copyright (c) 2018 - 2022, Oleg Malyavkin
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# DEBUG_TAB redefine this "  " if you need, example: const DEBUG_TAB = "\t"
const DEBUG_TAB : String = "  "

enum PB_ERR {
	NO_ERRORS = 0,
	VARINT_NOT_FOUND = -1,
	REPEATED_COUNT_NOT_FOUND = -2,
	REPEATED_COUNT_MISMATCH = -3,
	LENGTHDEL_SIZE_NOT_FOUND = -4,
	LENGTHDEL_SIZE_MISMATCH = -5,
	PACKAGE_SIZE_MISMATCH = -6,
	UNDEFINED_STATE = -7,
	PARSE_INCOMPLETE = -8,
	REQUIRED_FIELDS = -9
}

enum PB_DATA_TYPE {
	INT32 = 0,
	SINT32 = 1,
	UINT32 = 2,
	INT64 = 3,
	SINT64 = 4,
	UINT64 = 5,
	BOOL = 6,
	ENUM = 7,
	FIXED32 = 8,
	SFIXED32 = 9,
	FLOAT = 10,
	FIXED64 = 11,
	SFIXED64 = 12,
	DOUBLE = 13,
	STRING = 14,
	BYTES = 15,
	MESSAGE = 16,
	MAP = 17
}

const DEFAULT_VALUES_2 = {
	PB_DATA_TYPE.INT32: null,
	PB_DATA_TYPE.SINT32: null,
	PB_DATA_TYPE.UINT32: null,
	PB_DATA_TYPE.INT64: null,
	PB_DATA_TYPE.SINT64: null,
	PB_DATA_TYPE.UINT64: null,
	PB_DATA_TYPE.BOOL: null,
	PB_DATA_TYPE.ENUM: null,
	PB_DATA_TYPE.FIXED32: null,
	PB_DATA_TYPE.SFIXED32: null,
	PB_DATA_TYPE.FLOAT: null,
	PB_DATA_TYPE.FIXED64: null,
	PB_DATA_TYPE.SFIXED64: null,
	PB_DATA_TYPE.DOUBLE: null,
	PB_DATA_TYPE.STRING: null,
	PB_DATA_TYPE.BYTES: null,
	PB_DATA_TYPE.MESSAGE: null,
	PB_DATA_TYPE.MAP: null
}

const DEFAULT_VALUES_3 = {
	PB_DATA_TYPE.INT32: 0,
	PB_DATA_TYPE.SINT32: 0,
	PB_DATA_TYPE.UINT32: 0,
	PB_DATA_TYPE.INT64: 0,
	PB_DATA_TYPE.SINT64: 0,
	PB_DATA_TYPE.UINT64: 0,
	PB_DATA_TYPE.BOOL: false,
	PB_DATA_TYPE.ENUM: 0,
	PB_DATA_TYPE.FIXED32: 0,
	PB_DATA_TYPE.SFIXED32: 0,
	PB_DATA_TYPE.FLOAT: 0.0,
	PB_DATA_TYPE.FIXED64: 0,
	PB_DATA_TYPE.SFIXED64: 0,
	PB_DATA_TYPE.DOUBLE: 0.0,
	PB_DATA_TYPE.STRING: "",
	PB_DATA_TYPE.BYTES: [],
	PB_DATA_TYPE.MESSAGE: null,
	PB_DATA_TYPE.MAP: []
}

enum PB_TYPE {
	VARINT = 0,
	FIX64 = 1,
	LENGTHDEL = 2,
	STARTGROUP = 3,
	ENDGROUP = 4,
	FIX32 = 5,
	UNDEFINED = 8
}

enum PB_RULE {
	OPTIONAL = 0,
	REQUIRED = 1,
	REPEATED = 2,
	RESERVED = 3
}

enum PB_SERVICE_STATE {
	FILLED = 0,
	UNFILLED = 1
}

class PBField:
	func _init(a_name : String, a_type : int, a_rule : int, a_tag : int, packed : bool, a_value = null):
		name = a_name
		type = a_type
		rule = a_rule
		tag = a_tag
		option_packed = packed
		value = a_value
		
	var name : String
	var type : int
	var rule : int
	var tag : int
	var option_packed : bool
	var value
	var is_map_field : bool = false
	var option_default : bool = false

class PBTypeTag:
	var ok : bool = false
	var type : int
	var tag : int
	var offset : int

class PBServiceField:
	var field : PBField
	var func_ref = null
	var state : int = PB_SERVICE_STATE.UNFILLED

class PBPacker:
	static func convert_signed(n : int) -> int:
		if n < -2147483648:
			return (n << 1) ^ (n >> 63)
		else:
			return (n << 1) ^ (n >> 31)

	static func deconvert_signed(n : int) -> int:
		if n & 0x01:
			return ~(n >> 1)
		else:
			return (n >> 1)

	static func pack_varint(value) -> PoolByteArray:
		var varint : PoolByteArray = PoolByteArray()
		if typeof(value) == TYPE_BOOL:
			if value:
				value = 1
			else:
				value = 0
		for _i in range(9):
			var b = value & 0x7F
			value >>= 7
			if value:
				varint.append(b | 0x80)
			else:
				varint.append(b)
				break
		if varint.size() == 9 && varint[8] == 0xFF:
			varint.append(0x01)
		return varint

	static func pack_bytes(value, count : int, data_type : int) -> PoolByteArray:
		var bytes : PoolByteArray = PoolByteArray()
		if data_type == PB_DATA_TYPE.FLOAT:
			var spb : StreamPeerBuffer = StreamPeerBuffer.new()
			spb.put_float(value)
			bytes = spb.get_data_array()
		elif data_type == PB_DATA_TYPE.DOUBLE:
			var spb : StreamPeerBuffer = StreamPeerBuffer.new()
			spb.put_double(value)
			bytes = spb.get_data_array()
		else:
			for _i in range(count):
				bytes.append(value & 0xFF)
				value >>= 8
		return bytes

	static func unpack_bytes(bytes : PoolByteArray, index : int, count : int, data_type : int):
		var value = 0
		if data_type == PB_DATA_TYPE.FLOAT:
			var spb : StreamPeerBuffer = StreamPeerBuffer.new()
			for i in range(index, count + index):
				spb.put_u8(bytes[i])
			spb.seek(0)
			value = spb.get_float()
		elif data_type == PB_DATA_TYPE.DOUBLE:
			var spb : StreamPeerBuffer = StreamPeerBuffer.new()
			for i in range(index, count + index):
				spb.put_u8(bytes[i])
			spb.seek(0)
			value = spb.get_double()
		else:
			for i in range(index + count - 1, index - 1, -1):
				value |= (bytes[i] & 0xFF)
				if i != index:
					value <<= 8
		return value

	static func unpack_varint(varint_bytes) -> int:
		var value : int = 0
		for i in range(varint_bytes.size() - 1, -1, -1):
			value |= varint_bytes[i] & 0x7F
			if i != 0:
				value <<= 7
		return value

	static func pack_type_tag(type : int, tag : int) -> PoolByteArray:
		return pack_varint((tag << 3) | type)

	static func isolate_varint(bytes : PoolByteArray, index : int) -> PoolByteArray:
		var result : PoolByteArray = PoolByteArray()
		for i in range(index, bytes.size()):
			result.append(bytes[i])
			if !(bytes[i] & 0x80):
				break
		return result

	static func unpack_type_tag(bytes : PoolByteArray, index : int) -> PBTypeTag:
		var varint_bytes : PoolByteArray = isolate_varint(bytes, index)
		var result : PBTypeTag = PBTypeTag.new()
		if varint_bytes.size() != 0:
			result.ok = true
			result.offset = varint_bytes.size()
			var unpacked : int = unpack_varint(varint_bytes)
			result.type = unpacked & 0x07
			result.tag = unpacked >> 3
		return result

	static func pack_length_delimeted(type : int, tag : int, bytes : PoolByteArray) -> PoolByteArray:
		var result : PoolByteArray = pack_type_tag(type, tag)
		result.append_array(pack_varint(bytes.size()))
		result.append_array(bytes)
		return result

	static func pb_type_from_data_type(data_type : int) -> int:
		if data_type == PB_DATA_TYPE.INT32 || data_type == PB_DATA_TYPE.SINT32 || data_type == PB_DATA_TYPE.UINT32 || data_type == PB_DATA_TYPE.INT64 || data_type == PB_DATA_TYPE.SINT64 || data_type == PB_DATA_TYPE.UINT64 || data_type == PB_DATA_TYPE.BOOL || data_type == PB_DATA_TYPE.ENUM:
			return PB_TYPE.VARINT
		elif data_type == PB_DATA_TYPE.FIXED32 || data_type == PB_DATA_TYPE.SFIXED32 || data_type == PB_DATA_TYPE.FLOAT:
			return PB_TYPE.FIX32
		elif data_type == PB_DATA_TYPE.FIXED64 || data_type == PB_DATA_TYPE.SFIXED64 || data_type == PB_DATA_TYPE.DOUBLE:
			return PB_TYPE.FIX64
		elif data_type == PB_DATA_TYPE.STRING || data_type == PB_DATA_TYPE.BYTES || data_type == PB_DATA_TYPE.MESSAGE || data_type == PB_DATA_TYPE.MAP:
			return PB_TYPE.LENGTHDEL
		else:
			return PB_TYPE.UNDEFINED

	static func pack_field(field : PBField) -> PoolByteArray:
		var type : int = pb_type_from_data_type(field.type)
		var type_copy : int = type
		if field.rule == PB_RULE.REPEATED && field.option_packed:
			type = PB_TYPE.LENGTHDEL
		var head : PoolByteArray = pack_type_tag(type, field.tag)
		var data : PoolByteArray = PoolByteArray()
		if type == PB_TYPE.VARINT:
			var value
			if field.rule == PB_RULE.REPEATED:
				for v in field.value:
					data.append_array(head)
					if field.type == PB_DATA_TYPE.SINT32 || field.type == PB_DATA_TYPE.SINT64:
						value = convert_signed(v)
					else:
						value = v
					data.append_array(pack_varint(value))
				return data
			else:
				if field.type == PB_DATA_TYPE.SINT32 || field.type == PB_DATA_TYPE.SINT64:
					value = convert_signed(field.value)
				else:
					value = field.value
				data = pack_varint(value)
		elif type == PB_TYPE.FIX32:
			if field.rule == PB_RULE.REPEATED:
				for v in field.value:
					data.append_array(head)
					data.append_array(pack_bytes(v, 4, field.type))
				return data
			else:
				data.append_array(pack_bytes(field.value, 4, field.type))
		elif type == PB_TYPE.FIX64:
			if field.rule == PB_RULE.REPEATED:
				for v in field.value:
					data.append_array(head)
					data.append_array(pack_bytes(v, 8, field.type))
				return data
			else:
				data.append_array(pack_bytes(field.value, 8, field.type))
		elif type == PB_TYPE.LENGTHDEL:
			if field.rule == PB_RULE.REPEATED:
				if type_copy == PB_TYPE.VARINT:
					if field.type == PB_DATA_TYPE.SINT32 || field.type == PB_DATA_TYPE.SINT64:
						var signed_value : int
						for v in field.value:
							signed_value = convert_signed(v)
							data.append_array(pack_varint(signed_value))
					else:
						for v in field.value:
							data.append_array(pack_varint(v))
					return pack_length_delimeted(type, field.tag, data)
				elif type_copy == PB_TYPE.FIX32:
					for v in field.value:
						data.append_array(pack_bytes(v, 4, field.type))
					return pack_length_delimeted(type, field.tag, data)
				elif type_copy == PB_TYPE.FIX64:
					for v in field.value:
						data.append_array(pack_bytes(v, 8, field.type))
					return pack_length_delimeted(type, field.tag, data)
				elif field.type == PB_DATA_TYPE.STRING:
					for v in field.value:
						var obj = v.to_utf8()
						data.append_array(pack_length_delimeted(type, field.tag, obj))
					return data
				elif field.type == PB_DATA_TYPE.BYTES:
					for v in field.value:
						data.append_array(pack_length_delimeted(type, field.tag, v))
					return data
				elif typeof(field.value[0]) == TYPE_OBJECT:
					for v in field.value:
						var obj : PoolByteArray = v.to_bytes()
						data.append_array(pack_length_delimeted(type, field.tag, obj))
					return data
			else:
				if field.type == PB_DATA_TYPE.STRING:
					var str_bytes : PoolByteArray = field.value.to_utf8()
					if PROTO_VERSION == 2 || (PROTO_VERSION == 3 && str_bytes.size() > 0):
						data.append_array(str_bytes)
						return pack_length_delimeted(type, field.tag, data)
				if field.type == PB_DATA_TYPE.BYTES:
					if PROTO_VERSION == 2 || (PROTO_VERSION == 3 && field.value.size() > 0):
						data.append_array(field.value)
						return pack_length_delimeted(type, field.tag, data)
				elif typeof(field.value) == TYPE_OBJECT:
					var obj : PoolByteArray = field.value.to_bytes()
					if obj.size() > 0:
						data.append_array(obj)
					return pack_length_delimeted(type, field.tag, data)
				else:
					pass
		if data.size() > 0:
			head.append_array(data)
			return head
		else:
			return data

	static func unpack_field(bytes : PoolByteArray, offset : int, field : PBField, type : int, message_func_ref) -> int:
		if field.rule == PB_RULE.REPEATED && type != PB_TYPE.LENGTHDEL && field.option_packed:
			var count = isolate_varint(bytes, offset)
			if count.size() > 0:
				offset += count.size()
				count = unpack_varint(count)
				if type == PB_TYPE.VARINT:
					var val
					var counter = offset + count
					while offset < counter:
						val = isolate_varint(bytes, offset)
						if val.size() > 0:
							offset += val.size()
							val = unpack_varint(val)
							if field.type == PB_DATA_TYPE.SINT32 || field.type == PB_DATA_TYPE.SINT64:
								val = deconvert_signed(val)
							elif field.type == PB_DATA_TYPE.BOOL:
								if val:
									val = true
								else:
									val = false
							field.value.append(val)
						else:
							return PB_ERR.REPEATED_COUNT_MISMATCH
					return offset
				elif type == PB_TYPE.FIX32 || type == PB_TYPE.FIX64:
					var type_size
					if type == PB_TYPE.FIX32:
						type_size = 4
					else:
						type_size = 8
					var val
					var counter = offset + count
					while offset < counter:
						if (offset + type_size) > bytes.size():
							return PB_ERR.REPEATED_COUNT_MISMATCH
						val = unpack_bytes(bytes, offset, type_size, field.type)
						offset += type_size
						field.value.append(val)
					return offset
			else:
				return PB_ERR.REPEATED_COUNT_NOT_FOUND
		else:
			if type == PB_TYPE.VARINT:
				var val = isolate_varint(bytes, offset)
				if val.size() > 0:
					offset += val.size()
					val = unpack_varint(val)
					if field.type == PB_DATA_TYPE.SINT32 || field.type == PB_DATA_TYPE.SINT64:
						val = deconvert_signed(val)
					elif field.type == PB_DATA_TYPE.BOOL:
						if val:
							val = true
						else:
							val = false
					if field.rule == PB_RULE.REPEATED:
						field.value.append(val)
					else:
						field.value = val
				else:
					return PB_ERR.VARINT_NOT_FOUND
				return offset
			elif type == PB_TYPE.FIX32 || type == PB_TYPE.FIX64:
				var type_size
				if type == PB_TYPE.FIX32:
					type_size = 4
				else:
					type_size = 8
				var val
				if (offset + type_size) > bytes.size():
					return PB_ERR.REPEATED_COUNT_MISMATCH
				val = unpack_bytes(bytes, offset, type_size, field.type)
				offset += type_size
				if field.rule == PB_RULE.REPEATED:
					field.value.append(val)
				else:
					field.value = val
				return offset
			elif type == PB_TYPE.LENGTHDEL:
				var inner_size = isolate_varint(bytes, offset)
				if inner_size.size() > 0:
					offset += inner_size.size()
					inner_size = unpack_varint(inner_size)
					if inner_size >= 0:
						if inner_size + offset > bytes.size():
							return PB_ERR.LENGTHDEL_SIZE_MISMATCH
						if message_func_ref != null:
							var message = message_func_ref.call_func()
							if inner_size > 0:
								var sub_offset = message.from_bytes(bytes, offset, inner_size + offset)
								if sub_offset > 0:
									if sub_offset - offset >= inner_size:
										offset = sub_offset
										return offset
									else:
										return PB_ERR.LENGTHDEL_SIZE_MISMATCH
								return sub_offset
							else:
								return offset
						elif field.type == PB_DATA_TYPE.STRING:
							var str_bytes : PoolByteArray = PoolByteArray()
							for i in range(offset, inner_size + offset):
								str_bytes.append(bytes[i])
							if field.rule == PB_RULE.REPEATED:
								field.value.append(str_bytes.get_string_from_utf8())
							else:
								field.value = str_bytes.get_string_from_utf8()
							return offset + inner_size
						elif field.type == PB_DATA_TYPE.BYTES:
							var val_bytes : PoolByteArray = PoolByteArray()
							for i in range(offset, inner_size + offset):
								val_bytes.append(bytes[i])
							if field.rule == PB_RULE.REPEATED:
								field.value.append(val_bytes)
							else:
								field.value = val_bytes
							return offset + inner_size
					else:
						return PB_ERR.LENGTHDEL_SIZE_NOT_FOUND
				else:
					return PB_ERR.LENGTHDEL_SIZE_NOT_FOUND
		return PB_ERR.UNDEFINED_STATE

	static func unpack_message(data, bytes : PoolByteArray, offset : int, limit : int) -> int:
		while true:
			var tt : PBTypeTag = unpack_type_tag(bytes, offset)
			if tt.ok:
				offset += tt.offset
				if data.has(tt.tag):
					var service : PBServiceField = data[tt.tag]
					var type : int = pb_type_from_data_type(service.field.type)
					if type == tt.type || (tt.type == PB_TYPE.LENGTHDEL && service.field.rule == PB_RULE.REPEATED && service.field.option_packed):
						var res : int = unpack_field(bytes, offset, service.field, type, service.func_ref)
						if res > 0:
							service.state = PB_SERVICE_STATE.FILLED
							offset = res
							if offset == limit:
								return offset
							elif offset > limit:
								return PB_ERR.PACKAGE_SIZE_MISMATCH
						elif res < 0:
							return res
						else:
							break
			else:
				return offset
		return PB_ERR.UNDEFINED_STATE

	static func pack_message(data) -> PoolByteArray:
		var DEFAULT_VALUES
		if PROTO_VERSION == 2:
			DEFAULT_VALUES = DEFAULT_VALUES_2
		elif PROTO_VERSION == 3:
			DEFAULT_VALUES = DEFAULT_VALUES_3
		var result : PoolByteArray = PoolByteArray()
		var keys : Array = data.keys()
		keys.sort()
		for i in keys:
			if data[i].field.value != null:
				if data[i].state == PB_SERVICE_STATE.UNFILLED \
				&& !data[i].field.is_map_field \
				&& typeof(data[i].field.value) == typeof(DEFAULT_VALUES[data[i].field.type]) \
				&& data[i].field.value == DEFAULT_VALUES[data[i].field.type]:
					continue
				elif data[i].field.rule == PB_RULE.REPEATED && data[i].field.value.size() == 0:
					continue
				result.append_array(pack_field(data[i].field))
			elif data[i].field.rule == PB_RULE.REQUIRED:
				print("Error: required field is not filled: Tag:", data[i].field.tag)
				return PoolByteArray()
		return result

	static func check_required(data) -> bool:
		var keys : Array = data.keys()
		for i in keys:
			if data[i].field.rule == PB_RULE.REQUIRED && data[i].state == PB_SERVICE_STATE.UNFILLED:
				return false
		return true

	static func construct_map(key_values):
		var result = {}
		for kv in key_values:
			result[kv.get_key()] = kv.get_value()
		return result
	
	static func tabulate(text : String, nesting : int) -> String:
		var tab : String = ""
		for _i in range(nesting):
			tab += DEBUG_TAB
		return tab + text
	
	static func value_to_string(value, field : PBField, nesting : int) -> String:
		var result : String = ""
		var text : String
		if field.type == PB_DATA_TYPE.MESSAGE:
			result += "{"
			nesting += 1
			text = message_to_string(value.data, nesting)
			if text != "":
				result += "\n" + text
				nesting -= 1
				result += tabulate("}", nesting)
			else:
				nesting -= 1
				result += "}"
		elif field.type == PB_DATA_TYPE.BYTES:
			result += "<"
			for i in range(value.size()):
				result += String(value[i])
				if i != (value.size() - 1):
					result += ", "
			result += ">"
		elif field.type == PB_DATA_TYPE.STRING:
			result += "\"" + value + "\""
		elif field.type == PB_DATA_TYPE.ENUM:
			result += "ENUM::" + String(value)
		else:
			result += String(value)
		return result
	
	static func field_to_string(field : PBField, nesting : int) -> String:
		var result : String = tabulate(field.name + ": ", nesting)
		if field.type == PB_DATA_TYPE.MAP:
			if field.value.size() > 0:
				result += "(\n"
				nesting += 1
				for i in range(field.value.size()):
					var local_key_value = field.value[i].data[1].field
					result += tabulate(value_to_string(local_key_value.value, local_key_value, nesting), nesting) + ": "
					local_key_value = field.value[i].data[2].field
					result += value_to_string(local_key_value.value, local_key_value, nesting)
					if i != (field.value.size() - 1):
						result += ","
					result += "\n"
				nesting -= 1
				result += tabulate(")", nesting)
			else:
				result += "()"
		elif field.rule == PB_RULE.REPEATED:
			if field.value.size() > 0:
				result += "[\n"
				nesting += 1
				for i in range(field.value.size()):
					result += tabulate(String(i) + ": ", nesting)
					result += value_to_string(field.value[i], field, nesting)
					if i != (field.value.size() - 1):
						result += ","
					result += "\n"
				nesting -= 1
				result += tabulate("]", nesting)
			else:
				result += "[]"
		else:
			result += value_to_string(field.value, field, nesting)
		result += ";\n"
		return result
		
	static func message_to_string(data, nesting : int = 0) -> String:
		var DEFAULT_VALUES
		if PROTO_VERSION == 2:
			DEFAULT_VALUES = DEFAULT_VALUES_2
		elif PROTO_VERSION == 3:
			DEFAULT_VALUES = DEFAULT_VALUES_3
		var result : String = ""
		var keys : Array = data.keys()
		keys.sort()
		for i in keys:
			if data[i].field.value != null:
				if data[i].state == PB_SERVICE_STATE.UNFILLED \
				&& !data[i].field.is_map_field \
				&& typeof(data[i].field.value) == typeof(DEFAULT_VALUES[data[i].field.type]) \
				&& data[i].field.value == DEFAULT_VALUES[data[i].field.type]:
					continue
				elif data[i].field.rule == PB_RULE.REPEATED && data[i].field.value.size() == 0:
					continue
				result += field_to_string(data[i].field, nesting)
			elif data[i].field.rule == PB_RULE.REQUIRED:
				result += data[i].field.name + ": " + "error"
		return result



############### USER DATA BEGIN ################


class Guildpoint:
	func _init():
		var service
		
		_category = PBField.new("category", PB_DATA_TYPE.MESSAGE, PB_RULE.REPEATED, 1, true, [])
		service = PBServiceField.new()
		service.field = _category
		service.func_ref = funcref(self, "add_category")
		data[_category.tag] = service
		
		_textures = PBField.new("textures", PB_DATA_TYPE.MESSAGE, PB_RULE.REPEATED, 2, true, [])
		service = PBServiceField.new()
		service.field = _textures
		service.func_ref = funcref(self, "add_textures")
		data[_textures.tag] = service
		
	var data = {}
	
	var _category: PBField
	func get_category() -> Array:
		return _category.value
	func clear_category() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_category.value = []
	func add_category() -> Category:
		var element = Category.new()
		_category.value.append(element)
		return element
	
	var _textures: PBField
	func get_textures() -> Array:
		return _textures.value
	func clear_textures() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_textures.value = []
	func add_textures() -> TextureData:
		var element = TextureData.new()
		_textures.value.append(element)
		return element
	
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
class TextureData:
	func _init():
		var service
		
		_filepath = PBField.new("filepath", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 1, true, DEFAULT_VALUES_3[PB_DATA_TYPE.STRING])
		service = PBServiceField.new()
		service.field = _filepath
		data[_filepath.tag] = service
		
	var data = {}
	
	var _filepath: PBField
	func get_filepath() -> String:
		return _filepath.value
	func clear_filepath() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_filepath.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
	func set_filepath(value : String) -> void:
		_filepath.value = value
	
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
class Category:
	func _init():
		var service
		
		_name = PBField.new("name", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 1, true, DEFAULT_VALUES_3[PB_DATA_TYPE.STRING])
		service = PBServiceField.new()
		service.field = _name
		data[_name.tag] = service
		
		_children = PBField.new("children", PB_DATA_TYPE.MESSAGE, PB_RULE.REPEATED, 2, true, [])
		service = PBServiceField.new()
		service.field = _children
		service.func_ref = funcref(self, "add_children")
		data[_children.tag] = service
		
		_icon = PBField.new("icon", PB_DATA_TYPE.MESSAGE, PB_RULE.REPEATED, 3, true, [])
		service = PBServiceField.new()
		service.field = _icon
		service.func_ref = funcref(self, "add_icon")
		data[_icon.tag] = service
		
		_trail = PBField.new("trail", PB_DATA_TYPE.MESSAGE, PB_RULE.REPEATED, 4, true, [])
		service = PBServiceField.new()
		service.field = _trail
		service.func_ref = funcref(self, "add_trail")
		data[_trail.tag] = service
		
		_is_separator = PBField.new("is_separator", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 5, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _is_separator
		data[_is_separator.tag] = service
		
		_is_hidden = PBField.new("is_hidden", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 6, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _is_hidden
		data[_is_hidden.tag] = service
		
		_tip_description = PBField.new("tip_description", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 7, true, DEFAULT_VALUES_3[PB_DATA_TYPE.STRING])
		service = PBServiceField.new()
		service.field = _tip_description
		data[_tip_description.tag] = service
		
		_id = PBField.new("id", PB_DATA_TYPE.BYTES, PB_RULE.OPTIONAL, 8, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BYTES])
		service = PBServiceField.new()
		service.field = _id
		data[_id.tag] = service
		
	var data = {}
	
	var _name: PBField
	func get_name() -> String:
		return _name.value
	func clear_name() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_name.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
	func set_name(value : String) -> void:
		_name.value = value
	
	var _children: PBField
	func get_children() -> Array:
		return _children.value
	func clear_children() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_children.value = []
	func add_children() -> Category:
		var element = Category.new()
		_children.value.append(element)
		return element
	
	var _icon: PBField
	func get_icon() -> Array:
		return _icon.value
	func clear_icon() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_icon.value = []
	func add_icon() -> Icon:
		var element = Icon.new()
		_icon.value.append(element)
		return element
	
	var _trail: PBField
	func get_trail() -> Array:
		return _trail.value
	func clear_trail() -> void:
		data[4].state = PB_SERVICE_STATE.UNFILLED
		_trail.value = []
	func add_trail() -> Trail:
		var element = Trail.new()
		_trail.value.append(element)
		return element
	
	var _is_separator: PBField
	func get_is_separator() -> bool:
		return _is_separator.value
	func clear_is_separator() -> void:
		data[5].state = PB_SERVICE_STATE.UNFILLED
		_is_separator.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_is_separator(value : bool) -> void:
		_is_separator.value = value
	
	var _is_hidden: PBField
	func get_is_hidden() -> bool:
		return _is_hidden.value
	func clear_is_hidden() -> void:
		data[6].state = PB_SERVICE_STATE.UNFILLED
		_is_hidden.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_is_hidden(value : bool) -> void:
		_is_hidden.value = value
	
	var _tip_description: PBField
	func get_tip_description() -> String:
		return _tip_description.value
	func clear_tip_description() -> void:
		data[7].state = PB_SERVICE_STATE.UNFILLED
		_tip_description.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
	func set_tip_description(value : String) -> void:
		_tip_description.value = value
	
	var _id: PBField
	func get_id() -> PoolByteArray:
		return _id.value
	func clear_id() -> void:
		data[8].state = PB_SERVICE_STATE.UNFILLED
		_id.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BYTES]
	func set_id(value : PoolByteArray) -> void:
		_id.value = value
	
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
class Icon:
	func _init():
		var service
		
		_texture_id = PBField.new("texture_id", PB_DATA_TYPE.UINT32, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.UINT32])
		service = PBServiceField.new()
		service.field = _texture_id
		data[_texture_id.tag] = service
		
		_guid = PBField.new("guid", PB_DATA_TYPE.BYTES, PB_RULE.OPTIONAL, 3, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BYTES])
		service = PBServiceField.new()
		service.field = _guid
		data[_guid.tag] = service
		
		_map_id = PBField.new("map_id", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 4, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
		service = PBServiceField.new()
		service.field = _map_id
		data[_map_id.tag] = service
		
		_distance_fade_end = PBField.new("distance_fade_end", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 5, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _distance_fade_end
		data[_distance_fade_end.tag] = service
		
		_distance_fade_start = PBField.new("distance_fade_start", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 6, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _distance_fade_start
		data[_distance_fade_start.tag] = service
		
		_height_offset = PBField.new("height_offset", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 7, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _height_offset
		data[_height_offset.tag] = service
		
		_position = PBField.new("position", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 8, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _position
		service.func_ref = funcref(self, "new_position")
		data[_position.tag] = service
		
		_trigger = PBField.new("trigger", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 9, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _trigger
		service.func_ref = funcref(self, "new_trigger")
		data[_trigger.tag] = service
		
		_euler_rotation = PBField.new("euler_rotation", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 10, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _euler_rotation
		service.func_ref = funcref(self, "new_euler_rotation")
		data[_euler_rotation.tag] = service
		
		_achievement_id = PBField.new("achievement_id", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 16, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
		service = PBServiceField.new()
		service.field = _achievement_id
		data[_achievement_id.tag] = service
		
		_achievement_bit_index = PBField.new("achievement_bit_index", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 17, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
		service = PBServiceField.new()
		service.field = _achievement_bit_index
		data[_achievement_bit_index.tag] = service
		
		_disable_player_cutout = PBField.new("disable_player_cutout", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 19, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _disable_player_cutout
		data[_disable_player_cutout.tag] = service
		
		_minimum_size_on_screen = PBField.new("minimum_size_on_screen", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 20, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
		service = PBServiceField.new()
		service.field = _minimum_size_on_screen
		data[_minimum_size_on_screen.tag] = service
		
		_map_display_size = PBField.new("map_display_size", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 21, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
		service = PBServiceField.new()
		service.field = _map_display_size
		data[_map_display_size.tag] = service
		
		_maximum_size_on_screen = PBField.new("maximum_size_on_screen", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 22, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
		service = PBServiceField.new()
		service.field = _maximum_size_on_screen
		data[_maximum_size_on_screen.tag] = service
		
		_constant_size_on_map = PBField.new("constant_size_on_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 23, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _constant_size_on_map
		data[_constant_size_on_map.tag] = service
		
		_tip_description = PBField.new("tip_description", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 24, true, DEFAULT_VALUES_3[PB_DATA_TYPE.STRING])
		service = PBServiceField.new()
		service.field = _tip_description
		data[_tip_description.tag] = service
		
		_tip_name = PBField.new("tip_name", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 25, true, DEFAULT_VALUES_3[PB_DATA_TYPE.STRING])
		service = PBServiceField.new()
		service.field = _tip_name
		data[_tip_name.tag] = service
		
		_rgba_color = PBField.new("rgba_color", PB_DATA_TYPE.FIXED32, PB_RULE.OPTIONAL, 26, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FIXED32])
		service = PBServiceField.new()
		service.field = _rgba_color
		data[_rgba_color.tag] = service
		
		_festival_filter = PBField.new("festival_filter", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 27, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _festival_filter
		service.func_ref = funcref(self, "new_festival_filter")
		data[_festival_filter.tag] = service
		
		_map_type_filter = PBField.new("map_type_filter", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 28, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _map_type_filter
		service.func_ref = funcref(self, "new_map_type_filter")
		data[_map_type_filter.tag] = service
		
		_mount_filter = PBField.new("mount_filter", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 29, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _mount_filter
		service.func_ref = funcref(self, "new_mount_filter")
		data[_mount_filter.tag] = service
		
		_profession_filter = PBField.new("profession_filter", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 30, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _profession_filter
		service.func_ref = funcref(self, "new_profession_filter")
		data[_profession_filter.tag] = service
		
		_specialization_filter = PBField.new("specialization_filter", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 31, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _specialization_filter
		service.func_ref = funcref(self, "new_specialization_filter")
		data[_specialization_filter.tag] = service
		
		_species_filter = PBField.new("species_filter", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 32, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _species_filter
		service.func_ref = funcref(self, "new_species_filter")
		data[_species_filter.tag] = service
		
		_cull_chirality = PBField.new("cull_chirality", PB_DATA_TYPE.ENUM, PB_RULE.OPTIONAL, 33, true, DEFAULT_VALUES_3[PB_DATA_TYPE.ENUM])
		service = PBServiceField.new()
		service.field = _cull_chirality
		data[_cull_chirality.tag] = service
		
		_is_hidden_ingame = PBField.new("is_hidden_ingame", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 34, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _is_hidden_ingame
		data[_is_hidden_ingame.tag] = service
		
		_is_hidden_on_map = PBField.new("is_hidden_on_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 35, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _is_hidden_on_map
		data[_is_hidden_on_map.tag] = service
		
		_is_hidden_on_minimap = PBField.new("is_hidden_on_minimap", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 36, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _is_hidden_on_minimap
		data[_is_hidden_on_minimap.tag] = service
		
		_tentative__scale = PBField.new("tentative__scale", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 2048, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _tentative__scale
		data[_tentative__scale.tag] = service
		
		_bhdraft__schedule = PBField.new("bhdraft__schedule", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 2052, true, DEFAULT_VALUES_3[PB_DATA_TYPE.STRING])
		service = PBServiceField.new()
		service.field = _bhdraft__schedule
		data[_bhdraft__schedule.tag] = service
		
		_bhdraft__schedule_duration = PBField.new("bhdraft__schedule_duration", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 2053, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _bhdraft__schedule_duration
		data[_bhdraft__schedule_duration.tag] = service
		
	var data = {}
	
	var _texture_id: PBField
	func get_texture_id() -> int:
		return _texture_id.value
	func clear_texture_id() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_texture_id.value = DEFAULT_VALUES_3[PB_DATA_TYPE.UINT32]
	func set_texture_id(value : int) -> void:
		_texture_id.value = value
	
	var _guid: PBField
	func get_guid() -> PoolByteArray:
		return _guid.value
	func clear_guid() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_guid.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BYTES]
	func set_guid(value : PoolByteArray) -> void:
		_guid.value = value
	
	var _map_id: PBField
	func get_map_id() -> int:
		return _map_id.value
	func clear_map_id() -> void:
		data[4].state = PB_SERVICE_STATE.UNFILLED
		_map_id.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
	func set_map_id(value : int) -> void:
		_map_id.value = value
	
	var _distance_fade_end: PBField
	func get_distance_fade_end() -> float:
		return _distance_fade_end.value
	func clear_distance_fade_end() -> void:
		data[5].state = PB_SERVICE_STATE.UNFILLED
		_distance_fade_end.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_distance_fade_end(value : float) -> void:
		_distance_fade_end.value = value
	
	var _distance_fade_start: PBField
	func get_distance_fade_start() -> float:
		return _distance_fade_start.value
	func clear_distance_fade_start() -> void:
		data[6].state = PB_SERVICE_STATE.UNFILLED
		_distance_fade_start.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_distance_fade_start(value : float) -> void:
		_distance_fade_start.value = value
	
	var _height_offset: PBField
	func get_height_offset() -> float:
		return _height_offset.value
	func clear_height_offset() -> void:
		data[7].state = PB_SERVICE_STATE.UNFILLED
		_height_offset.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_height_offset(value : float) -> void:
		_height_offset.value = value
	
	var _position: PBField
	func get_position() -> Position:
		return _position.value
	func clear_position() -> void:
		data[8].state = PB_SERVICE_STATE.UNFILLED
		_position.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_position() -> Position:
		_position.value = Position.new()
		return _position.value
	
	var _trigger: PBField
	func get_trigger() -> Trigger:
		return _trigger.value
	func clear_trigger() -> void:
		data[9].state = PB_SERVICE_STATE.UNFILLED
		_trigger.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_trigger() -> Trigger:
		_trigger.value = Trigger.new()
		return _trigger.value
	
	var _euler_rotation: PBField
	func get_euler_rotation() -> EulerRotation:
		return _euler_rotation.value
	func clear_euler_rotation() -> void:
		data[10].state = PB_SERVICE_STATE.UNFILLED
		_euler_rotation.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_euler_rotation() -> EulerRotation:
		_euler_rotation.value = EulerRotation.new()
		return _euler_rotation.value
	
	var _achievement_id: PBField
	func get_achievement_id() -> int:
		return _achievement_id.value
	func clear_achievement_id() -> void:
		data[16].state = PB_SERVICE_STATE.UNFILLED
		_achievement_id.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
	func set_achievement_id(value : int) -> void:
		_achievement_id.value = value
	
	var _achievement_bit_index: PBField
	func get_achievement_bit_index() -> int:
		return _achievement_bit_index.value
	func clear_achievement_bit_index() -> void:
		data[17].state = PB_SERVICE_STATE.UNFILLED
		_achievement_bit_index.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
	func set_achievement_bit_index(value : int) -> void:
		_achievement_bit_index.value = value
	
	var _disable_player_cutout: PBField
	func get_disable_player_cutout() -> bool:
		return _disable_player_cutout.value
	func clear_disable_player_cutout() -> void:
		data[19].state = PB_SERVICE_STATE.UNFILLED
		_disable_player_cutout.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_disable_player_cutout(value : bool) -> void:
		_disable_player_cutout.value = value
	
	var _minimum_size_on_screen: PBField
	func get_minimum_size_on_screen() -> int:
		return _minimum_size_on_screen.value
	func clear_minimum_size_on_screen() -> void:
		data[20].state = PB_SERVICE_STATE.UNFILLED
		_minimum_size_on_screen.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
	func set_minimum_size_on_screen(value : int) -> void:
		_minimum_size_on_screen.value = value
	
	var _map_display_size: PBField
	func get_map_display_size() -> int:
		return _map_display_size.value
	func clear_map_display_size() -> void:
		data[21].state = PB_SERVICE_STATE.UNFILLED
		_map_display_size.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
	func set_map_display_size(value : int) -> void:
		_map_display_size.value = value
	
	var _maximum_size_on_screen: PBField
	func get_maximum_size_on_screen() -> int:
		return _maximum_size_on_screen.value
	func clear_maximum_size_on_screen() -> void:
		data[22].state = PB_SERVICE_STATE.UNFILLED
		_maximum_size_on_screen.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
	func set_maximum_size_on_screen(value : int) -> void:
		_maximum_size_on_screen.value = value
	
	var _constant_size_on_map: PBField
	func get_constant_size_on_map() -> bool:
		return _constant_size_on_map.value
	func clear_constant_size_on_map() -> void:
		data[23].state = PB_SERVICE_STATE.UNFILLED
		_constant_size_on_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_constant_size_on_map(value : bool) -> void:
		_constant_size_on_map.value = value
	
	var _tip_description: PBField
	func get_tip_description() -> String:
		return _tip_description.value
	func clear_tip_description() -> void:
		data[24].state = PB_SERVICE_STATE.UNFILLED
		_tip_description.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
	func set_tip_description(value : String) -> void:
		_tip_description.value = value
	
	var _tip_name: PBField
	func get_tip_name() -> String:
		return _tip_name.value
	func clear_tip_name() -> void:
		data[25].state = PB_SERVICE_STATE.UNFILLED
		_tip_name.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
	func set_tip_name(value : String) -> void:
		_tip_name.value = value
	
	var _rgba_color: PBField
	func get_rgba_color() -> int:
		return _rgba_color.value
	func clear_rgba_color() -> void:
		data[26].state = PB_SERVICE_STATE.UNFILLED
		_rgba_color.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FIXED32]
	func set_rgba_color(value : int) -> void:
		_rgba_color.value = value
	
	var _festival_filter: PBField
	func get_festival_filter() -> FestivalFilter:
		return _festival_filter.value
	func clear_festival_filter() -> void:
		data[27].state = PB_SERVICE_STATE.UNFILLED
		_festival_filter.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_festival_filter() -> FestivalFilter:
		_festival_filter.value = FestivalFilter.new()
		return _festival_filter.value
	
	var _map_type_filter: PBField
	func get_map_type_filter() -> MapTypeFilter:
		return _map_type_filter.value
	func clear_map_type_filter() -> void:
		data[28].state = PB_SERVICE_STATE.UNFILLED
		_map_type_filter.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_map_type_filter() -> MapTypeFilter:
		_map_type_filter.value = MapTypeFilter.new()
		return _map_type_filter.value
	
	var _mount_filter: PBField
	func get_mount_filter() -> MountFilter:
		return _mount_filter.value
	func clear_mount_filter() -> void:
		data[29].state = PB_SERVICE_STATE.UNFILLED
		_mount_filter.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_mount_filter() -> MountFilter:
		_mount_filter.value = MountFilter.new()
		return _mount_filter.value
	
	var _profession_filter: PBField
	func get_profession_filter() -> ProfessionFilter:
		return _profession_filter.value
	func clear_profession_filter() -> void:
		data[30].state = PB_SERVICE_STATE.UNFILLED
		_profession_filter.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_profession_filter() -> ProfessionFilter:
		_profession_filter.value = ProfessionFilter.new()
		return _profession_filter.value
	
	var _specialization_filter: PBField
	func get_specialization_filter() -> SpecializationFilter:
		return _specialization_filter.value
	func clear_specialization_filter() -> void:
		data[31].state = PB_SERVICE_STATE.UNFILLED
		_specialization_filter.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_specialization_filter() -> SpecializationFilter:
		_specialization_filter.value = SpecializationFilter.new()
		return _specialization_filter.value
	
	var _species_filter: PBField
	func get_species_filter() -> SpeciesFilter:
		return _species_filter.value
	func clear_species_filter() -> void:
		data[32].state = PB_SERVICE_STATE.UNFILLED
		_species_filter.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_species_filter() -> SpeciesFilter:
		_species_filter.value = SpeciesFilter.new()
		return _species_filter.value
	
	var _cull_chirality: PBField
	func get_cull_chirality():
		return _cull_chirality.value
	func clear_cull_chirality() -> void:
		data[33].state = PB_SERVICE_STATE.UNFILLED
		_cull_chirality.value = DEFAULT_VALUES_3[PB_DATA_TYPE.ENUM]
	func set_cull_chirality(value) -> void:
		_cull_chirality.value = value
	
	var _is_hidden_ingame: PBField
	func get_is_hidden_ingame() -> bool:
		return _is_hidden_ingame.value
	func clear_is_hidden_ingame() -> void:
		data[34].state = PB_SERVICE_STATE.UNFILLED
		_is_hidden_ingame.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_is_hidden_ingame(value : bool) -> void:
		_is_hidden_ingame.value = value
	
	var _is_hidden_on_map: PBField
	func get_is_hidden_on_map() -> bool:
		return _is_hidden_on_map.value
	func clear_is_hidden_on_map() -> void:
		data[35].state = PB_SERVICE_STATE.UNFILLED
		_is_hidden_on_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_is_hidden_on_map(value : bool) -> void:
		_is_hidden_on_map.value = value
	
	var _is_hidden_on_minimap: PBField
	func get_is_hidden_on_minimap() -> bool:
		return _is_hidden_on_minimap.value
	func clear_is_hidden_on_minimap() -> void:
		data[36].state = PB_SERVICE_STATE.UNFILLED
		_is_hidden_on_minimap.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_is_hidden_on_minimap(value : bool) -> void:
		_is_hidden_on_minimap.value = value
	
	var _tentative__scale: PBField
	func get_tentative__scale() -> float:
		return _tentative__scale.value
	func clear_tentative__scale() -> void:
		data[2048].state = PB_SERVICE_STATE.UNFILLED
		_tentative__scale.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_tentative__scale(value : float) -> void:
		_tentative__scale.value = value
	
	var _bhdraft__schedule: PBField
	func get_bhdraft__schedule() -> String:
		return _bhdraft__schedule.value
	func clear_bhdraft__schedule() -> void:
		data[2052].state = PB_SERVICE_STATE.UNFILLED
		_bhdraft__schedule.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
	func set_bhdraft__schedule(value : String) -> void:
		_bhdraft__schedule.value = value
	
	var _bhdraft__schedule_duration: PBField
	func get_bhdraft__schedule_duration() -> float:
		return _bhdraft__schedule_duration.value
	func clear_bhdraft__schedule_duration() -> void:
		data[2053].state = PB_SERVICE_STATE.UNFILLED
		_bhdraft__schedule_duration.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_bhdraft__schedule_duration(value : float) -> void:
		_bhdraft__schedule_duration.value = value
	
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
class Trail:
	func _init():
		var service
		
		_texture_id = PBField.new("texture_id", PB_DATA_TYPE.UINT32, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.UINT32])
		service = PBServiceField.new()
		service.field = _texture_id
		data[_texture_id.tag] = service
		
		_guid = PBField.new("guid", PB_DATA_TYPE.BYTES, PB_RULE.OPTIONAL, 3, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BYTES])
		service = PBServiceField.new()
		service.field = _guid
		data[_guid.tag] = service
		
		_map_id = PBField.new("map_id", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 4, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
		service = PBServiceField.new()
		service.field = _map_id
		data[_map_id.tag] = service
		
		_distance_fade_end = PBField.new("distance_fade_end", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 5, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _distance_fade_end
		data[_distance_fade_end.tag] = service
		
		_distance_fade_start = PBField.new("distance_fade_start", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 6, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _distance_fade_start
		data[_distance_fade_start.tag] = service
		
		_trail_data = PBField.new("trail_data", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 7, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _trail_data
		service.func_ref = funcref(self, "new_trail_data")
		data[_trail_data.tag] = service
		
		_animation_speed = PBField.new("animation_speed", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 8, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _animation_speed
		data[_animation_speed.tag] = service
		
		_achievement_id = PBField.new("achievement_id", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 16, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
		service = PBServiceField.new()
		service.field = _achievement_id
		data[_achievement_id.tag] = service
		
		_achievement_bit_index = PBField.new("achievement_bit_index", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 17, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
		service = PBServiceField.new()
		service.field = _achievement_bit_index
		data[_achievement_bit_index.tag] = service
		
		_disable_player_cutout = PBField.new("disable_player_cutout", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 19, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _disable_player_cutout
		data[_disable_player_cutout.tag] = service
		
		_is_wall = PBField.new("is_wall", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 20, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _is_wall
		data[_is_wall.tag] = service
		
		_scale = PBField.new("scale", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 21, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _scale
		data[_scale.tag] = service
		
		_rgba_color = PBField.new("rgba_color", PB_DATA_TYPE.FIXED32, PB_RULE.OPTIONAL, 22, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FIXED32])
		service = PBServiceField.new()
		service.field = _rgba_color
		data[_rgba_color.tag] = service
		
		_festival_filter = PBField.new("festival_filter", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 23, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _festival_filter
		service.func_ref = funcref(self, "new_festival_filter")
		data[_festival_filter.tag] = service
		
		_map_type_filter = PBField.new("map_type_filter", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 24, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _map_type_filter
		service.func_ref = funcref(self, "new_map_type_filter")
		data[_map_type_filter.tag] = service
		
		_mount_filter = PBField.new("mount_filter", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 25, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _mount_filter
		service.func_ref = funcref(self, "new_mount_filter")
		data[_mount_filter.tag] = service
		
		_profession_filter = PBField.new("profession_filter", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 26, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _profession_filter
		service.func_ref = funcref(self, "new_profession_filter")
		data[_profession_filter.tag] = service
		
		_specialization_filter = PBField.new("specialization_filter", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 27, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _specialization_filter
		service.func_ref = funcref(self, "new_specialization_filter")
		data[_specialization_filter.tag] = service
		
		_species_filter = PBField.new("species_filter", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 28, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _species_filter
		service.func_ref = funcref(self, "new_species_filter")
		data[_species_filter.tag] = service
		
		_map_display_size = PBField.new("map_display_size", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 29, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
		service = PBServiceField.new()
		service.field = _map_display_size
		data[_map_display_size.tag] = service
		
		_cull_chirality = PBField.new("cull_chirality", PB_DATA_TYPE.ENUM, PB_RULE.OPTIONAL, 30, true, DEFAULT_VALUES_3[PB_DATA_TYPE.ENUM])
		service = PBServiceField.new()
		service.field = _cull_chirality
		data[_cull_chirality.tag] = service
		
		_is_hidden_ingame = PBField.new("is_hidden_ingame", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 31, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _is_hidden_ingame
		data[_is_hidden_ingame.tag] = service
		
		_is_hidden_on_map = PBField.new("is_hidden_on_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 32, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _is_hidden_on_map
		data[_is_hidden_on_map.tag] = service
		
		_is_hidden_on_minimap = PBField.new("is_hidden_on_minimap", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 33, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _is_hidden_on_minimap
		data[_is_hidden_on_minimap.tag] = service
		
		_bhdraft__schedule = PBField.new("bhdraft__schedule", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 2052, true, DEFAULT_VALUES_3[PB_DATA_TYPE.STRING])
		service = PBServiceField.new()
		service.field = _bhdraft__schedule
		data[_bhdraft__schedule.tag] = service
		
		_bhdraft__schedule_duration = PBField.new("bhdraft__schedule_duration", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 2053, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _bhdraft__schedule_duration
		data[_bhdraft__schedule_duration.tag] = service
		
	var data = {}
	
	var _texture_id: PBField
	func get_texture_id() -> int:
		return _texture_id.value
	func clear_texture_id() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_texture_id.value = DEFAULT_VALUES_3[PB_DATA_TYPE.UINT32]
	func set_texture_id(value : int) -> void:
		_texture_id.value = value
	
	var _guid: PBField
	func get_guid() -> PoolByteArray:
		return _guid.value
	func clear_guid() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_guid.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BYTES]
	func set_guid(value : PoolByteArray) -> void:
		_guid.value = value
	
	var _map_id: PBField
	func get_map_id() -> int:
		return _map_id.value
	func clear_map_id() -> void:
		data[4].state = PB_SERVICE_STATE.UNFILLED
		_map_id.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
	func set_map_id(value : int) -> void:
		_map_id.value = value
	
	var _distance_fade_end: PBField
	func get_distance_fade_end() -> float:
		return _distance_fade_end.value
	func clear_distance_fade_end() -> void:
		data[5].state = PB_SERVICE_STATE.UNFILLED
		_distance_fade_end.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_distance_fade_end(value : float) -> void:
		_distance_fade_end.value = value
	
	var _distance_fade_start: PBField
	func get_distance_fade_start() -> float:
		return _distance_fade_start.value
	func clear_distance_fade_start() -> void:
		data[6].state = PB_SERVICE_STATE.UNFILLED
		_distance_fade_start.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_distance_fade_start(value : float) -> void:
		_distance_fade_start.value = value
	
	var _trail_data: PBField
	func get_trail_data() -> TrailData:
		return _trail_data.value
	func clear_trail_data() -> void:
		data[7].state = PB_SERVICE_STATE.UNFILLED
		_trail_data.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_trail_data() -> TrailData:
		_trail_data.value = TrailData.new()
		return _trail_data.value
	
	var _animation_speed: PBField
	func get_animation_speed() -> float:
		return _animation_speed.value
	func clear_animation_speed() -> void:
		data[8].state = PB_SERVICE_STATE.UNFILLED
		_animation_speed.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_animation_speed(value : float) -> void:
		_animation_speed.value = value
	
	var _achievement_id: PBField
	func get_achievement_id() -> int:
		return _achievement_id.value
	func clear_achievement_id() -> void:
		data[16].state = PB_SERVICE_STATE.UNFILLED
		_achievement_id.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
	func set_achievement_id(value : int) -> void:
		_achievement_id.value = value
	
	var _achievement_bit_index: PBField
	func get_achievement_bit_index() -> int:
		return _achievement_bit_index.value
	func clear_achievement_bit_index() -> void:
		data[17].state = PB_SERVICE_STATE.UNFILLED
		_achievement_bit_index.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
	func set_achievement_bit_index(value : int) -> void:
		_achievement_bit_index.value = value
	
	var _disable_player_cutout: PBField
	func get_disable_player_cutout() -> bool:
		return _disable_player_cutout.value
	func clear_disable_player_cutout() -> void:
		data[19].state = PB_SERVICE_STATE.UNFILLED
		_disable_player_cutout.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_disable_player_cutout(value : bool) -> void:
		_disable_player_cutout.value = value
	
	var _is_wall: PBField
	func get_is_wall() -> bool:
		return _is_wall.value
	func clear_is_wall() -> void:
		data[20].state = PB_SERVICE_STATE.UNFILLED
		_is_wall.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_is_wall(value : bool) -> void:
		_is_wall.value = value
	
	var _scale: PBField
	func get_scale() -> float:
		return _scale.value
	func clear_scale() -> void:
		data[21].state = PB_SERVICE_STATE.UNFILLED
		_scale.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_scale(value : float) -> void:
		_scale.value = value
	
	var _rgba_color: PBField
	func get_rgba_color() -> int:
		return _rgba_color.value
	func clear_rgba_color() -> void:
		data[22].state = PB_SERVICE_STATE.UNFILLED
		_rgba_color.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FIXED32]
	func set_rgba_color(value : int) -> void:
		_rgba_color.value = value
	
	var _festival_filter: PBField
	func get_festival_filter() -> FestivalFilter:
		return _festival_filter.value
	func clear_festival_filter() -> void:
		data[23].state = PB_SERVICE_STATE.UNFILLED
		_festival_filter.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_festival_filter() -> FestivalFilter:
		_festival_filter.value = FestivalFilter.new()
		return _festival_filter.value
	
	var _map_type_filter: PBField
	func get_map_type_filter() -> MapTypeFilter:
		return _map_type_filter.value
	func clear_map_type_filter() -> void:
		data[24].state = PB_SERVICE_STATE.UNFILLED
		_map_type_filter.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_map_type_filter() -> MapTypeFilter:
		_map_type_filter.value = MapTypeFilter.new()
		return _map_type_filter.value
	
	var _mount_filter: PBField
	func get_mount_filter() -> MountFilter:
		return _mount_filter.value
	func clear_mount_filter() -> void:
		data[25].state = PB_SERVICE_STATE.UNFILLED
		_mount_filter.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_mount_filter() -> MountFilter:
		_mount_filter.value = MountFilter.new()
		return _mount_filter.value
	
	var _profession_filter: PBField
	func get_profession_filter() -> ProfessionFilter:
		return _profession_filter.value
	func clear_profession_filter() -> void:
		data[26].state = PB_SERVICE_STATE.UNFILLED
		_profession_filter.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_profession_filter() -> ProfessionFilter:
		_profession_filter.value = ProfessionFilter.new()
		return _profession_filter.value
	
	var _specialization_filter: PBField
	func get_specialization_filter() -> SpecializationFilter:
		return _specialization_filter.value
	func clear_specialization_filter() -> void:
		data[27].state = PB_SERVICE_STATE.UNFILLED
		_specialization_filter.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_specialization_filter() -> SpecializationFilter:
		_specialization_filter.value = SpecializationFilter.new()
		return _specialization_filter.value
	
	var _species_filter: PBField
	func get_species_filter() -> SpeciesFilter:
		return _species_filter.value
	func clear_species_filter() -> void:
		data[28].state = PB_SERVICE_STATE.UNFILLED
		_species_filter.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_species_filter() -> SpeciesFilter:
		_species_filter.value = SpeciesFilter.new()
		return _species_filter.value
	
	var _map_display_size: PBField
	func get_map_display_size() -> int:
		return _map_display_size.value
	func clear_map_display_size() -> void:
		data[29].state = PB_SERVICE_STATE.UNFILLED
		_map_display_size.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
	func set_map_display_size(value : int) -> void:
		_map_display_size.value = value
	
	var _cull_chirality: PBField
	func get_cull_chirality():
		return _cull_chirality.value
	func clear_cull_chirality() -> void:
		data[30].state = PB_SERVICE_STATE.UNFILLED
		_cull_chirality.value = DEFAULT_VALUES_3[PB_DATA_TYPE.ENUM]
	func set_cull_chirality(value) -> void:
		_cull_chirality.value = value
	
	var _is_hidden_ingame: PBField
	func get_is_hidden_ingame() -> bool:
		return _is_hidden_ingame.value
	func clear_is_hidden_ingame() -> void:
		data[31].state = PB_SERVICE_STATE.UNFILLED
		_is_hidden_ingame.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_is_hidden_ingame(value : bool) -> void:
		_is_hidden_ingame.value = value
	
	var _is_hidden_on_map: PBField
	func get_is_hidden_on_map() -> bool:
		return _is_hidden_on_map.value
	func clear_is_hidden_on_map() -> void:
		data[32].state = PB_SERVICE_STATE.UNFILLED
		_is_hidden_on_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_is_hidden_on_map(value : bool) -> void:
		_is_hidden_on_map.value = value
	
	var _is_hidden_on_minimap: PBField
	func get_is_hidden_on_minimap() -> bool:
		return _is_hidden_on_minimap.value
	func clear_is_hidden_on_minimap() -> void:
		data[33].state = PB_SERVICE_STATE.UNFILLED
		_is_hidden_on_minimap.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_is_hidden_on_minimap(value : bool) -> void:
		_is_hidden_on_minimap.value = value
	
	var _bhdraft__schedule: PBField
	func get_bhdraft__schedule() -> String:
		return _bhdraft__schedule.value
	func clear_bhdraft__schedule() -> void:
		data[2052].state = PB_SERVICE_STATE.UNFILLED
		_bhdraft__schedule.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
	func set_bhdraft__schedule(value : String) -> void:
		_bhdraft__schedule.value = value
	
	var _bhdraft__schedule_duration: PBField
	func get_bhdraft__schedule_duration() -> float:
		return _bhdraft__schedule_duration.value
	func clear_bhdraft__schedule_duration() -> void:
		data[2053].state = PB_SERVICE_STATE.UNFILLED
		_bhdraft__schedule_duration.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_bhdraft__schedule_duration(value : float) -> void:
		_bhdraft__schedule_duration.value = value
	
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
class Position:
	func _init():
		var service
		
		_x = PBField.new("x", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 1, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _x
		data[_x.tag] = service
		
		_y = PBField.new("y", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _y
		data[_y.tag] = service
		
		_z = PBField.new("z", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 3, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _z
		data[_z.tag] = service
		
	var data = {}
	
	var _x: PBField
	func get_x() -> float:
		return _x.value
	func clear_x() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_x.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_x(value : float) -> void:
		_x.value = value
	
	var _y: PBField
	func get_y() -> float:
		return _y.value
	func clear_y() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_y.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_y(value : float) -> void:
		_y.value = value
	
	var _z: PBField
	func get_z() -> float:
		return _z.value
	func clear_z() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_z.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_z(value : float) -> void:
		_z.value = value
	
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
class EulerRotation:
	func _init():
		var service
		
		_x = PBField.new("x", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 1, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _x
		data[_x.tag] = service
		
		_y = PBField.new("y", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _y
		data[_y.tag] = service
		
		_z = PBField.new("z", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 3, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _z
		data[_z.tag] = service
		
	var data = {}
	
	var _x: PBField
	func get_x() -> float:
		return _x.value
	func clear_x() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_x.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_x(value : float) -> void:
		_x.value = value
	
	var _y: PBField
	func get_y() -> float:
		return _y.value
	func clear_y() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_y.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_y(value : float) -> void:
		_y.value = value
	
	var _z: PBField
	func get_z() -> float:
		return _z.value
	func clear_z() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_z.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_z(value : float) -> void:
		_z.value = value
	
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
class Trigger:
	func _init():
		var service
		
		_auto_trigger = PBField.new("auto_trigger", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 1, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _auto_trigger
		data[_auto_trigger.tag] = service
		
		_bounce_delay = PBField.new("bounce_delay", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _bounce_delay
		data[_bounce_delay.tag] = service
		
		_bounce_duration = PBField.new("bounce_duration", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 3, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _bounce_duration
		data[_bounce_duration.tag] = service
		
		_bounce_height = PBField.new("bounce_height", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 4, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _bounce_height
		data[_bounce_height.tag] = service
		
		_action_copy_clipboard = PBField.new("action_copy_clipboard", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 5, true, DEFAULT_VALUES_3[PB_DATA_TYPE.STRING])
		service = PBServiceField.new()
		service.field = _action_copy_clipboard
		data[_action_copy_clipboard.tag] = service
		
		_action_copy_message = PBField.new("action_copy_message", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 6, true, DEFAULT_VALUES_3[PB_DATA_TYPE.STRING])
		service = PBServiceField.new()
		service.field = _action_copy_message
		data[_action_copy_message.tag] = service
		
		_has_countdown = PBField.new("has_countdown", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 7, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _has_countdown
		data[_has_countdown.tag] = service
		
		_action_info_message = PBField.new("action_info_message", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 8, true, DEFAULT_VALUES_3[PB_DATA_TYPE.STRING])
		service = PBServiceField.new()
		service.field = _action_info_message
		data[_action_info_message.tag] = service
		
		_invert_display = PBField.new("invert_display", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 9, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _invert_display
		data[_invert_display.tag] = service
		
		_reset_length = PBField.new("reset_length", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 10, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _reset_length
		data[_reset_length.tag] = service
		
		_range = PBField.new("range", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 11, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _range
		data[_range.tag] = service
		
		_action_hide_category = PBField.new("action_hide_category", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 12, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _action_hide_category
		service.func_ref = funcref(self, "new_action_hide_category")
		data[_action_hide_category.tag] = service
		
		_action_show_category = PBField.new("action_show_category", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 13, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _action_show_category
		service.func_ref = funcref(self, "new_action_show_category")
		data[_action_show_category.tag] = service
		
		_action_toggle_category = PBField.new("action_toggle_category", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 14, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _action_toggle_category
		service.func_ref = funcref(self, "new_action_toggle_category")
		data[_action_toggle_category.tag] = service
		
		_reset_behavior = PBField.new("reset_behavior", PB_DATA_TYPE.ENUM, PB_RULE.OPTIONAL, 15, true, DEFAULT_VALUES_3[PB_DATA_TYPE.ENUM])
		service = PBServiceField.new()
		service.field = _reset_behavior
		data[_reset_behavior.tag] = service
		
	var data = {}
	
	var _auto_trigger: PBField
	func get_auto_trigger() -> bool:
		return _auto_trigger.value
	func clear_auto_trigger() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_auto_trigger.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_auto_trigger(value : bool) -> void:
		_auto_trigger.value = value
	
	var _bounce_delay: PBField
	func get_bounce_delay() -> float:
		return _bounce_delay.value
	func clear_bounce_delay() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_bounce_delay.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_bounce_delay(value : float) -> void:
		_bounce_delay.value = value
	
	var _bounce_duration: PBField
	func get_bounce_duration() -> float:
		return _bounce_duration.value
	func clear_bounce_duration() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_bounce_duration.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_bounce_duration(value : float) -> void:
		_bounce_duration.value = value
	
	var _bounce_height: PBField
	func get_bounce_height() -> float:
		return _bounce_height.value
	func clear_bounce_height() -> void:
		data[4].state = PB_SERVICE_STATE.UNFILLED
		_bounce_height.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_bounce_height(value : float) -> void:
		_bounce_height.value = value
	
	var _action_copy_clipboard: PBField
	func get_action_copy_clipboard() -> String:
		return _action_copy_clipboard.value
	func clear_action_copy_clipboard() -> void:
		data[5].state = PB_SERVICE_STATE.UNFILLED
		_action_copy_clipboard.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
	func set_action_copy_clipboard(value : String) -> void:
		_action_copy_clipboard.value = value
	
	var _action_copy_message: PBField
	func get_action_copy_message() -> String:
		return _action_copy_message.value
	func clear_action_copy_message() -> void:
		data[6].state = PB_SERVICE_STATE.UNFILLED
		_action_copy_message.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
	func set_action_copy_message(value : String) -> void:
		_action_copy_message.value = value
	
	var _has_countdown: PBField
	func get_has_countdown() -> bool:
		return _has_countdown.value
	func clear_has_countdown() -> void:
		data[7].state = PB_SERVICE_STATE.UNFILLED
		_has_countdown.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_has_countdown(value : bool) -> void:
		_has_countdown.value = value
	
	var _action_info_message: PBField
	func get_action_info_message() -> String:
		return _action_info_message.value
	func clear_action_info_message() -> void:
		data[8].state = PB_SERVICE_STATE.UNFILLED
		_action_info_message.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
	func set_action_info_message(value : String) -> void:
		_action_info_message.value = value
	
	var _invert_display: PBField
	func get_invert_display() -> bool:
		return _invert_display.value
	func clear_invert_display() -> void:
		data[9].state = PB_SERVICE_STATE.UNFILLED
		_invert_display.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_invert_display(value : bool) -> void:
		_invert_display.value = value
	
	var _reset_length: PBField
	func get_reset_length() -> float:
		return _reset_length.value
	func clear_reset_length() -> void:
		data[10].state = PB_SERVICE_STATE.UNFILLED
		_reset_length.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_reset_length(value : float) -> void:
		_reset_length.value = value
	
	var _range: PBField
	func get_range() -> float:
		return _range.value
	func clear_range() -> void:
		data[11].state = PB_SERVICE_STATE.UNFILLED
		_range.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_range(value : float) -> void:
		_range.value = value
	
	var _action_hide_category: PBField
	func get_action_hide_category() -> Category:
		return _action_hide_category.value
	func clear_action_hide_category() -> void:
		data[12].state = PB_SERVICE_STATE.UNFILLED
		_action_hide_category.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_action_hide_category() -> Category:
		_action_hide_category.value = Category.new()
		return _action_hide_category.value
	
	var _action_show_category: PBField
	func get_action_show_category() -> Category:
		return _action_show_category.value
	func clear_action_show_category() -> void:
		data[13].state = PB_SERVICE_STATE.UNFILLED
		_action_show_category.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_action_show_category() -> Category:
		_action_show_category.value = Category.new()
		return _action_show_category.value
	
	var _action_toggle_category: PBField
	func get_action_toggle_category() -> Category:
		return _action_toggle_category.value
	func clear_action_toggle_category() -> void:
		data[14].state = PB_SERVICE_STATE.UNFILLED
		_action_toggle_category.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_action_toggle_category() -> Category:
		_action_toggle_category.value = Category.new()
		return _action_toggle_category.value
	
	var _reset_behavior: PBField
	func get_reset_behavior():
		return _reset_behavior.value
	func clear_reset_behavior() -> void:
		data[15].state = PB_SERVICE_STATE.UNFILLED
		_reset_behavior.value = DEFAULT_VALUES_3[PB_DATA_TYPE.ENUM]
	func set_reset_behavior(value) -> void:
		_reset_behavior.value = value
	
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
enum CullChirality {
	none = 0,
	clockwise = 1,
	counter_clockwise = 2
}

enum ResetBehavior {
	always_visible = 0,
	map_change = 1,
	daily_reset = 2,
	never = 3,
	timer = 4,
	map_reset = 5,
	instance_change = 6,
	daily_reset_per_character = 7,
	weekly_reset = 8
}

class FestivalFilter:
	func _init():
		var service
		
		_dragonbash = PBField.new("dragonbash", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 1, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _dragonbash
		data[_dragonbash.tag] = service
		
		_festival_of_the_four_winds = PBField.new("festival_of_the_four_winds", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _festival_of_the_four_winds
		data[_festival_of_the_four_winds.tag] = service
		
		_halloween = PBField.new("halloween", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 3, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _halloween
		data[_halloween.tag] = service
		
		_lunar_new_year = PBField.new("lunar_new_year", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 4, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _lunar_new_year
		data[_lunar_new_year.tag] = service
		
		_super_adventure_festival = PBField.new("super_adventure_festival", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 5, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _super_adventure_festival
		data[_super_adventure_festival.tag] = service
		
		_wintersday = PBField.new("wintersday", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 6, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _wintersday
		data[_wintersday.tag] = service
		
		_none = PBField.new("none", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 7, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _none
		data[_none.tag] = service
		
	var data = {}
	
	var _dragonbash: PBField
	func get_dragonbash() -> bool:
		return _dragonbash.value
	func clear_dragonbash() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_dragonbash.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_dragonbash(value : bool) -> void:
		_dragonbash.value = value
	
	var _festival_of_the_four_winds: PBField
	func get_festival_of_the_four_winds() -> bool:
		return _festival_of_the_four_winds.value
	func clear_festival_of_the_four_winds() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_festival_of_the_four_winds.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_festival_of_the_four_winds(value : bool) -> void:
		_festival_of_the_four_winds.value = value
	
	var _halloween: PBField
	func get_halloween() -> bool:
		return _halloween.value
	func clear_halloween() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_halloween.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_halloween(value : bool) -> void:
		_halloween.value = value
	
	var _lunar_new_year: PBField
	func get_lunar_new_year() -> bool:
		return _lunar_new_year.value
	func clear_lunar_new_year() -> void:
		data[4].state = PB_SERVICE_STATE.UNFILLED
		_lunar_new_year.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_lunar_new_year(value : bool) -> void:
		_lunar_new_year.value = value
	
	var _super_adventure_festival: PBField
	func get_super_adventure_festival() -> bool:
		return _super_adventure_festival.value
	func clear_super_adventure_festival() -> void:
		data[5].state = PB_SERVICE_STATE.UNFILLED
		_super_adventure_festival.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_super_adventure_festival(value : bool) -> void:
		_super_adventure_festival.value = value
	
	var _wintersday: PBField
	func get_wintersday() -> bool:
		return _wintersday.value
	func clear_wintersday() -> void:
		data[6].state = PB_SERVICE_STATE.UNFILLED
		_wintersday.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_wintersday(value : bool) -> void:
		_wintersday.value = value
	
	var _none: PBField
	func get_none() -> bool:
		return _none.value
	func clear_none() -> void:
		data[7].state = PB_SERVICE_STATE.UNFILLED
		_none.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_none(value : bool) -> void:
		_none.value = value
	
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
class MapTypeFilter:
	func _init():
		var service
		
		_unknown_map = PBField.new("unknown_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 1, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _unknown_map
		data[_unknown_map.tag] = service
		
		_redirect_map = PBField.new("redirect_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _redirect_map
		data[_redirect_map.tag] = service
		
		_character_create_map = PBField.new("character_create_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 3, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _character_create_map
		data[_character_create_map.tag] = service
		
		_pvp_map = PBField.new("pvp_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 4, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _pvp_map
		data[_pvp_map.tag] = service
		
		_gvg_map = PBField.new("gvg_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 5, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _gvg_map
		data[_gvg_map.tag] = service
		
		_instance_map = PBField.new("instance_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 6, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _instance_map
		data[_instance_map.tag] = service
		
		_public_map = PBField.new("public_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 7, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _public_map
		data[_public_map.tag] = service
		
		_tournament_map = PBField.new("tournament_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 8, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _tournament_map
		data[_tournament_map.tag] = service
		
		_tutorial_map = PBField.new("tutorial_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 9, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _tutorial_map
		data[_tutorial_map.tag] = service
		
		_user_tournament_map = PBField.new("user_tournament_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 10, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _user_tournament_map
		data[_user_tournament_map.tag] = service
		
		_center_map = PBField.new("center_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 11, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _center_map
		data[_center_map.tag] = service
		
		_eternal_battlegrounds_map = PBField.new("eternal_battlegrounds_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 12, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _eternal_battlegrounds_map
		data[_eternal_battlegrounds_map.tag] = service
		
		_bluehome_map = PBField.new("bluehome_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 13, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _bluehome_map
		data[_bluehome_map.tag] = service
		
		_blue_borderlands_map = PBField.new("blue_borderlands_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 14, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _blue_borderlands_map
		data[_blue_borderlands_map.tag] = service
		
		_green_home_map = PBField.new("green_home_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 15, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _green_home_map
		data[_green_home_map.tag] = service
		
		_green_borderlands_map = PBField.new("green_borderlands_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 16, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _green_borderlands_map
		data[_green_borderlands_map.tag] = service
		
		_red_home_map = PBField.new("red_home_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 17, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _red_home_map
		data[_red_home_map.tag] = service
		
		_red_borderlands_map = PBField.new("red_borderlands_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 18, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _red_borderlands_map
		data[_red_borderlands_map.tag] = service
		
		_fortunes_vale_map = PBField.new("fortunes_vale_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 19, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _fortunes_vale_map
		data[_fortunes_vale_map.tag] = service
		
		_jump_puzzle_map = PBField.new("jump_puzzle_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 20, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _jump_puzzle_map
		data[_jump_puzzle_map.tag] = service
		
		_obsidian_sanctum_map = PBField.new("obsidian_sanctum_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 21, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _obsidian_sanctum_map
		data[_obsidian_sanctum_map.tag] = service
		
		_edge_of_the_mists_map = PBField.new("edge_of_the_mists_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 22, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _edge_of_the_mists_map
		data[_edge_of_the_mists_map.tag] = service
		
		_public_mini_map = PBField.new("public_mini_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 23, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _public_mini_map
		data[_public_mini_map.tag] = service
		
		_wvw_lounge_map = PBField.new("wvw_lounge_map", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 24, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _wvw_lounge_map
		data[_wvw_lounge_map.tag] = service
		
	var data = {}
	
	var _unknown_map: PBField
	func get_unknown_map() -> bool:
		return _unknown_map.value
	func clear_unknown_map() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_unknown_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_unknown_map(value : bool) -> void:
		_unknown_map.value = value
	
	var _redirect_map: PBField
	func get_redirect_map() -> bool:
		return _redirect_map.value
	func clear_redirect_map() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_redirect_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_redirect_map(value : bool) -> void:
		_redirect_map.value = value
	
	var _character_create_map: PBField
	func get_character_create_map() -> bool:
		return _character_create_map.value
	func clear_character_create_map() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_character_create_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_character_create_map(value : bool) -> void:
		_character_create_map.value = value
	
	var _pvp_map: PBField
	func get_pvp_map() -> bool:
		return _pvp_map.value
	func clear_pvp_map() -> void:
		data[4].state = PB_SERVICE_STATE.UNFILLED
		_pvp_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_pvp_map(value : bool) -> void:
		_pvp_map.value = value
	
	var _gvg_map: PBField
	func get_gvg_map() -> bool:
		return _gvg_map.value
	func clear_gvg_map() -> void:
		data[5].state = PB_SERVICE_STATE.UNFILLED
		_gvg_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_gvg_map(value : bool) -> void:
		_gvg_map.value = value
	
	var _instance_map: PBField
	func get_instance_map() -> bool:
		return _instance_map.value
	func clear_instance_map() -> void:
		data[6].state = PB_SERVICE_STATE.UNFILLED
		_instance_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_instance_map(value : bool) -> void:
		_instance_map.value = value
	
	var _public_map: PBField
	func get_public_map() -> bool:
		return _public_map.value
	func clear_public_map() -> void:
		data[7].state = PB_SERVICE_STATE.UNFILLED
		_public_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_public_map(value : bool) -> void:
		_public_map.value = value
	
	var _tournament_map: PBField
	func get_tournament_map() -> bool:
		return _tournament_map.value
	func clear_tournament_map() -> void:
		data[8].state = PB_SERVICE_STATE.UNFILLED
		_tournament_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_tournament_map(value : bool) -> void:
		_tournament_map.value = value
	
	var _tutorial_map: PBField
	func get_tutorial_map() -> bool:
		return _tutorial_map.value
	func clear_tutorial_map() -> void:
		data[9].state = PB_SERVICE_STATE.UNFILLED
		_tutorial_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_tutorial_map(value : bool) -> void:
		_tutorial_map.value = value
	
	var _user_tournament_map: PBField
	func get_user_tournament_map() -> bool:
		return _user_tournament_map.value
	func clear_user_tournament_map() -> void:
		data[10].state = PB_SERVICE_STATE.UNFILLED
		_user_tournament_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_user_tournament_map(value : bool) -> void:
		_user_tournament_map.value = value
	
	var _center_map: PBField
	func get_center_map() -> bool:
		return _center_map.value
	func clear_center_map() -> void:
		data[11].state = PB_SERVICE_STATE.UNFILLED
		_center_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_center_map(value : bool) -> void:
		_center_map.value = value
	
	var _eternal_battlegrounds_map: PBField
	func get_eternal_battlegrounds_map() -> bool:
		return _eternal_battlegrounds_map.value
	func clear_eternal_battlegrounds_map() -> void:
		data[12].state = PB_SERVICE_STATE.UNFILLED
		_eternal_battlegrounds_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_eternal_battlegrounds_map(value : bool) -> void:
		_eternal_battlegrounds_map.value = value
	
	var _bluehome_map: PBField
	func get_bluehome_map() -> bool:
		return _bluehome_map.value
	func clear_bluehome_map() -> void:
		data[13].state = PB_SERVICE_STATE.UNFILLED
		_bluehome_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_bluehome_map(value : bool) -> void:
		_bluehome_map.value = value
	
	var _blue_borderlands_map: PBField
	func get_blue_borderlands_map() -> bool:
		return _blue_borderlands_map.value
	func clear_blue_borderlands_map() -> void:
		data[14].state = PB_SERVICE_STATE.UNFILLED
		_blue_borderlands_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_blue_borderlands_map(value : bool) -> void:
		_blue_borderlands_map.value = value
	
	var _green_home_map: PBField
	func get_green_home_map() -> bool:
		return _green_home_map.value
	func clear_green_home_map() -> void:
		data[15].state = PB_SERVICE_STATE.UNFILLED
		_green_home_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_green_home_map(value : bool) -> void:
		_green_home_map.value = value
	
	var _green_borderlands_map: PBField
	func get_green_borderlands_map() -> bool:
		return _green_borderlands_map.value
	func clear_green_borderlands_map() -> void:
		data[16].state = PB_SERVICE_STATE.UNFILLED
		_green_borderlands_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_green_borderlands_map(value : bool) -> void:
		_green_borderlands_map.value = value
	
	var _red_home_map: PBField
	func get_red_home_map() -> bool:
		return _red_home_map.value
	func clear_red_home_map() -> void:
		data[17].state = PB_SERVICE_STATE.UNFILLED
		_red_home_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_red_home_map(value : bool) -> void:
		_red_home_map.value = value
	
	var _red_borderlands_map: PBField
	func get_red_borderlands_map() -> bool:
		return _red_borderlands_map.value
	func clear_red_borderlands_map() -> void:
		data[18].state = PB_SERVICE_STATE.UNFILLED
		_red_borderlands_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_red_borderlands_map(value : bool) -> void:
		_red_borderlands_map.value = value
	
	var _fortunes_vale_map: PBField
	func get_fortunes_vale_map() -> bool:
		return _fortunes_vale_map.value
	func clear_fortunes_vale_map() -> void:
		data[19].state = PB_SERVICE_STATE.UNFILLED
		_fortunes_vale_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_fortunes_vale_map(value : bool) -> void:
		_fortunes_vale_map.value = value
	
	var _jump_puzzle_map: PBField
	func get_jump_puzzle_map() -> bool:
		return _jump_puzzle_map.value
	func clear_jump_puzzle_map() -> void:
		data[20].state = PB_SERVICE_STATE.UNFILLED
		_jump_puzzle_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_jump_puzzle_map(value : bool) -> void:
		_jump_puzzle_map.value = value
	
	var _obsidian_sanctum_map: PBField
	func get_obsidian_sanctum_map() -> bool:
		return _obsidian_sanctum_map.value
	func clear_obsidian_sanctum_map() -> void:
		data[21].state = PB_SERVICE_STATE.UNFILLED
		_obsidian_sanctum_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_obsidian_sanctum_map(value : bool) -> void:
		_obsidian_sanctum_map.value = value
	
	var _edge_of_the_mists_map: PBField
	func get_edge_of_the_mists_map() -> bool:
		return _edge_of_the_mists_map.value
	func clear_edge_of_the_mists_map() -> void:
		data[22].state = PB_SERVICE_STATE.UNFILLED
		_edge_of_the_mists_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_edge_of_the_mists_map(value : bool) -> void:
		_edge_of_the_mists_map.value = value
	
	var _public_mini_map: PBField
	func get_public_mini_map() -> bool:
		return _public_mini_map.value
	func clear_public_mini_map() -> void:
		data[23].state = PB_SERVICE_STATE.UNFILLED
		_public_mini_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_public_mini_map(value : bool) -> void:
		_public_mini_map.value = value
	
	var _wvw_lounge_map: PBField
	func get_wvw_lounge_map() -> bool:
		return _wvw_lounge_map.value
	func clear_wvw_lounge_map() -> void:
		data[24].state = PB_SERVICE_STATE.UNFILLED
		_wvw_lounge_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_wvw_lounge_map(value : bool) -> void:
		_wvw_lounge_map.value = value
	
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
class MountFilter:
	func _init():
		var service
		
		_raptor = PBField.new("raptor", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 1, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _raptor
		data[_raptor.tag] = service
		
		_springer = PBField.new("springer", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _springer
		data[_springer.tag] = service
		
		_skimmer = PBField.new("skimmer", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 3, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _skimmer
		data[_skimmer.tag] = service
		
		_jackal = PBField.new("jackal", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 4, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _jackal
		data[_jackal.tag] = service
		
		_griffon = PBField.new("griffon", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 5, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _griffon
		data[_griffon.tag] = service
		
		_roller_beetle = PBField.new("roller_beetle", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 6, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _roller_beetle
		data[_roller_beetle.tag] = service
		
		_warclaw = PBField.new("warclaw", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 7, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _warclaw
		data[_warclaw.tag] = service
		
		_skyscale = PBField.new("skyscale", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 8, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _skyscale
		data[_skyscale.tag] = service
		
		_skiff = PBField.new("skiff", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 9, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _skiff
		data[_skiff.tag] = service
		
		_seige_turtle = PBField.new("seige_turtle", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 10, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _seige_turtle
		data[_seige_turtle.tag] = service
		
	var data = {}
	
	var _raptor: PBField
	func get_raptor() -> bool:
		return _raptor.value
	func clear_raptor() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_raptor.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_raptor(value : bool) -> void:
		_raptor.value = value
	
	var _springer: PBField
	func get_springer() -> bool:
		return _springer.value
	func clear_springer() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_springer.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_springer(value : bool) -> void:
		_springer.value = value
	
	var _skimmer: PBField
	func get_skimmer() -> bool:
		return _skimmer.value
	func clear_skimmer() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_skimmer.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_skimmer(value : bool) -> void:
		_skimmer.value = value
	
	var _jackal: PBField
	func get_jackal() -> bool:
		return _jackal.value
	func clear_jackal() -> void:
		data[4].state = PB_SERVICE_STATE.UNFILLED
		_jackal.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_jackal(value : bool) -> void:
		_jackal.value = value
	
	var _griffon: PBField
	func get_griffon() -> bool:
		return _griffon.value
	func clear_griffon() -> void:
		data[5].state = PB_SERVICE_STATE.UNFILLED
		_griffon.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_griffon(value : bool) -> void:
		_griffon.value = value
	
	var _roller_beetle: PBField
	func get_roller_beetle() -> bool:
		return _roller_beetle.value
	func clear_roller_beetle() -> void:
		data[6].state = PB_SERVICE_STATE.UNFILLED
		_roller_beetle.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_roller_beetle(value : bool) -> void:
		_roller_beetle.value = value
	
	var _warclaw: PBField
	func get_warclaw() -> bool:
		return _warclaw.value
	func clear_warclaw() -> void:
		data[7].state = PB_SERVICE_STATE.UNFILLED
		_warclaw.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_warclaw(value : bool) -> void:
		_warclaw.value = value
	
	var _skyscale: PBField
	func get_skyscale() -> bool:
		return _skyscale.value
	func clear_skyscale() -> void:
		data[8].state = PB_SERVICE_STATE.UNFILLED
		_skyscale.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_skyscale(value : bool) -> void:
		_skyscale.value = value
	
	var _skiff: PBField
	func get_skiff() -> bool:
		return _skiff.value
	func clear_skiff() -> void:
		data[9].state = PB_SERVICE_STATE.UNFILLED
		_skiff.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_skiff(value : bool) -> void:
		_skiff.value = value
	
	var _seige_turtle: PBField
	func get_seige_turtle() -> bool:
		return _seige_turtle.value
	func clear_seige_turtle() -> void:
		data[10].state = PB_SERVICE_STATE.UNFILLED
		_seige_turtle.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_seige_turtle(value : bool) -> void:
		_seige_turtle.value = value
	
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
class ProfessionFilter:
	func _init():
		var service
		
		_guardian = PBField.new("guardian", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 1, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _guardian
		data[_guardian.tag] = service
		
		_warrior = PBField.new("warrior", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _warrior
		data[_warrior.tag] = service
		
		_engineer = PBField.new("engineer", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 3, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _engineer
		data[_engineer.tag] = service
		
		_ranger = PBField.new("ranger", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 4, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _ranger
		data[_ranger.tag] = service
		
		_thief = PBField.new("thief", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 5, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _thief
		data[_thief.tag] = service
		
		_elementalist = PBField.new("elementalist", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 6, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _elementalist
		data[_elementalist.tag] = service
		
		_mesmer = PBField.new("mesmer", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 7, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _mesmer
		data[_mesmer.tag] = service
		
		_necromancer = PBField.new("necromancer", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 8, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _necromancer
		data[_necromancer.tag] = service
		
		_revenant = PBField.new("revenant", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 9, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _revenant
		data[_revenant.tag] = service
		
	var data = {}
	
	var _guardian: PBField
	func get_guardian() -> bool:
		return _guardian.value
	func clear_guardian() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_guardian.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_guardian(value : bool) -> void:
		_guardian.value = value
	
	var _warrior: PBField
	func get_warrior() -> bool:
		return _warrior.value
	func clear_warrior() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_warrior.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_warrior(value : bool) -> void:
		_warrior.value = value
	
	var _engineer: PBField
	func get_engineer() -> bool:
		return _engineer.value
	func clear_engineer() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_engineer.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_engineer(value : bool) -> void:
		_engineer.value = value
	
	var _ranger: PBField
	func get_ranger() -> bool:
		return _ranger.value
	func clear_ranger() -> void:
		data[4].state = PB_SERVICE_STATE.UNFILLED
		_ranger.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_ranger(value : bool) -> void:
		_ranger.value = value
	
	var _thief: PBField
	func get_thief() -> bool:
		return _thief.value
	func clear_thief() -> void:
		data[5].state = PB_SERVICE_STATE.UNFILLED
		_thief.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_thief(value : bool) -> void:
		_thief.value = value
	
	var _elementalist: PBField
	func get_elementalist() -> bool:
		return _elementalist.value
	func clear_elementalist() -> void:
		data[6].state = PB_SERVICE_STATE.UNFILLED
		_elementalist.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_elementalist(value : bool) -> void:
		_elementalist.value = value
	
	var _mesmer: PBField
	func get_mesmer() -> bool:
		return _mesmer.value
	func clear_mesmer() -> void:
		data[7].state = PB_SERVICE_STATE.UNFILLED
		_mesmer.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_mesmer(value : bool) -> void:
		_mesmer.value = value
	
	var _necromancer: PBField
	func get_necromancer() -> bool:
		return _necromancer.value
	func clear_necromancer() -> void:
		data[8].state = PB_SERVICE_STATE.UNFILLED
		_necromancer.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_necromancer(value : bool) -> void:
		_necromancer.value = value
	
	var _revenant: PBField
	func get_revenant() -> bool:
		return _revenant.value
	func clear_revenant() -> void:
		data[9].state = PB_SERVICE_STATE.UNFILLED
		_revenant.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_revenant(value : bool) -> void:
		_revenant.value = value
	
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
class SpecializationFilter:
	func _init():
		var service
		
		_elementalist_tempest = PBField.new("elementalist_tempest", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 1, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _elementalist_tempest
		data[_elementalist_tempest.tag] = service
		
		_engineer_scrapper = PBField.new("engineer_scrapper", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _engineer_scrapper
		data[_engineer_scrapper.tag] = service
		
		_guardian_dragonhunter = PBField.new("guardian_dragonhunter", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 3, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _guardian_dragonhunter
		data[_guardian_dragonhunter.tag] = service
		
		_mesmer_chronomancer = PBField.new("mesmer_chronomancer", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 4, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _mesmer_chronomancer
		data[_mesmer_chronomancer.tag] = service
		
		_necromancer_reaper = PBField.new("necromancer_reaper", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 5, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _necromancer_reaper
		data[_necromancer_reaper.tag] = service
		
		_ranger_druid = PBField.new("ranger_druid", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 6, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _ranger_druid
		data[_ranger_druid.tag] = service
		
		_revenant_herald = PBField.new("revenant_herald", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 7, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _revenant_herald
		data[_revenant_herald.tag] = service
		
		_thief_daredevil = PBField.new("thief_daredevil", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 8, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _thief_daredevil
		data[_thief_daredevil.tag] = service
		
		_warrior_berserker = PBField.new("warrior_berserker", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 9, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _warrior_berserker
		data[_warrior_berserker.tag] = service
		
		_elementalist_weaver = PBField.new("elementalist_weaver", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 10, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _elementalist_weaver
		data[_elementalist_weaver.tag] = service
		
		_engineer_holosmith = PBField.new("engineer_holosmith", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 11, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _engineer_holosmith
		data[_engineer_holosmith.tag] = service
		
		_guardian_firebrand = PBField.new("guardian_firebrand", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 12, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _guardian_firebrand
		data[_guardian_firebrand.tag] = service
		
		_mesmer_mirage = PBField.new("mesmer_mirage", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 13, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _mesmer_mirage
		data[_mesmer_mirage.tag] = service
		
		_necromancer_scourge = PBField.new("necromancer_scourge", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 14, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _necromancer_scourge
		data[_necromancer_scourge.tag] = service
		
		_ranger_soulbeast = PBField.new("ranger_soulbeast", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 15, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _ranger_soulbeast
		data[_ranger_soulbeast.tag] = service
		
		_revenant_renegade = PBField.new("revenant_renegade", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 16, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _revenant_renegade
		data[_revenant_renegade.tag] = service
		
		_thief_deadeye = PBField.new("thief_deadeye", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 17, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _thief_deadeye
		data[_thief_deadeye.tag] = service
		
		_warrior_spellbreaker = PBField.new("warrior_spellbreaker", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 18, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _warrior_spellbreaker
		data[_warrior_spellbreaker.tag] = service
		
		_elementalist_catalyst = PBField.new("elementalist_catalyst", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 19, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _elementalist_catalyst
		data[_elementalist_catalyst.tag] = service
		
		_engineer_mechanist = PBField.new("engineer_mechanist", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 20, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _engineer_mechanist
		data[_engineer_mechanist.tag] = service
		
		_guardian_willbender = PBField.new("guardian_willbender", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 21, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _guardian_willbender
		data[_guardian_willbender.tag] = service
		
		_mesmer_virtuoso = PBField.new("mesmer_virtuoso", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 22, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _mesmer_virtuoso
		data[_mesmer_virtuoso.tag] = service
		
		_necromancer_harbinger = PBField.new("necromancer_harbinger", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 23, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _necromancer_harbinger
		data[_necromancer_harbinger.tag] = service
		
		_ranger_untamed = PBField.new("ranger_untamed", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 24, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _ranger_untamed
		data[_ranger_untamed.tag] = service
		
		_revenant_vindicator = PBField.new("revenant_vindicator", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 25, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _revenant_vindicator
		data[_revenant_vindicator.tag] = service
		
		_thief_specter = PBField.new("thief_specter", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 26, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _thief_specter
		data[_thief_specter.tag] = service
		
		_warrior_bladesworn = PBField.new("warrior_bladesworn", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 27, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _warrior_bladesworn
		data[_warrior_bladesworn.tag] = service
		
		_elementalist_air = PBField.new("elementalist_air", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 28, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _elementalist_air
		data[_elementalist_air.tag] = service
		
		_elementalist_arcane = PBField.new("elementalist_arcane", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 29, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _elementalist_arcane
		data[_elementalist_arcane.tag] = service
		
		_elementalist_earth = PBField.new("elementalist_earth", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 30, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _elementalist_earth
		data[_elementalist_earth.tag] = service
		
		_elementalist_fire = PBField.new("elementalist_fire", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 31, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _elementalist_fire
		data[_elementalist_fire.tag] = service
		
		_elementalist_water = PBField.new("elementalist_water", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 32, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _elementalist_water
		data[_elementalist_water.tag] = service
		
		_engineer_alchemy = PBField.new("engineer_alchemy", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 33, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _engineer_alchemy
		data[_engineer_alchemy.tag] = service
		
		_engineer_explosives = PBField.new("engineer_explosives", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 34, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _engineer_explosives
		data[_engineer_explosives.tag] = service
		
		_engineer_firearms = PBField.new("engineer_firearms", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 35, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _engineer_firearms
		data[_engineer_firearms.tag] = service
		
		_engineer_inventions = PBField.new("engineer_inventions", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 36, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _engineer_inventions
		data[_engineer_inventions.tag] = service
		
		_engineer_tools = PBField.new("engineer_tools", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 37, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _engineer_tools
		data[_engineer_tools.tag] = service
		
		_guardian_honor = PBField.new("guardian_honor", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 38, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _guardian_honor
		data[_guardian_honor.tag] = service
		
		_guardian_radiance = PBField.new("guardian_radiance", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 39, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _guardian_radiance
		data[_guardian_radiance.tag] = service
		
		_guardian_valor = PBField.new("guardian_valor", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 40, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _guardian_valor
		data[_guardian_valor.tag] = service
		
		_guardian_virtues = PBField.new("guardian_virtues", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 41, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _guardian_virtues
		data[_guardian_virtues.tag] = service
		
		_guardian_zeal = PBField.new("guardian_zeal", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 42, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _guardian_zeal
		data[_guardian_zeal.tag] = service
		
		_mesmer_chaos = PBField.new("mesmer_chaos", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 43, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _mesmer_chaos
		data[_mesmer_chaos.tag] = service
		
		_mesmer_domination = PBField.new("mesmer_domination", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 44, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _mesmer_domination
		data[_mesmer_domination.tag] = service
		
		_mesmer_dueling = PBField.new("mesmer_dueling", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 45, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _mesmer_dueling
		data[_mesmer_dueling.tag] = service
		
		_mesmer_illusions = PBField.new("mesmer_illusions", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 46, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _mesmer_illusions
		data[_mesmer_illusions.tag] = service
		
		_mesmer_inspiration = PBField.new("mesmer_inspiration", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 47, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _mesmer_inspiration
		data[_mesmer_inspiration.tag] = service
		
		_necromancer_blood_magic = PBField.new("necromancer_blood_magic", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 48, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _necromancer_blood_magic
		data[_necromancer_blood_magic.tag] = service
		
		_necromancer_curses = PBField.new("necromancer_curses", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 49, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _necromancer_curses
		data[_necromancer_curses.tag] = service
		
		_necromancer_death_magic = PBField.new("necromancer_death_magic", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 50, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _necromancer_death_magic
		data[_necromancer_death_magic.tag] = service
		
		_necromancer_soul_reaping = PBField.new("necromancer_soul_reaping", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 51, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _necromancer_soul_reaping
		data[_necromancer_soul_reaping.tag] = service
		
		_necromancer_spite = PBField.new("necromancer_spite", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 52, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _necromancer_spite
		data[_necromancer_spite.tag] = service
		
		_ranger_beastmastery = PBField.new("ranger_beastmastery", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 53, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _ranger_beastmastery
		data[_ranger_beastmastery.tag] = service
		
		_ranger_marksmanship = PBField.new("ranger_marksmanship", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 54, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _ranger_marksmanship
		data[_ranger_marksmanship.tag] = service
		
		_ranger_nature_magic = PBField.new("ranger_nature_magic", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 55, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _ranger_nature_magic
		data[_ranger_nature_magic.tag] = service
		
		_ranger_skirmishing = PBField.new("ranger_skirmishing", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 56, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _ranger_skirmishing
		data[_ranger_skirmishing.tag] = service
		
		_ranger_wilderness_survival = PBField.new("ranger_wilderness_survival", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 57, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _ranger_wilderness_survival
		data[_ranger_wilderness_survival.tag] = service
		
		_revenant_corruption = PBField.new("revenant_corruption", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 58, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _revenant_corruption
		data[_revenant_corruption.tag] = service
		
		_revenant_devastation = PBField.new("revenant_devastation", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 59, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _revenant_devastation
		data[_revenant_devastation.tag] = service
		
		_revenant_invocation = PBField.new("revenant_invocation", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 60, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _revenant_invocation
		data[_revenant_invocation.tag] = service
		
		_revenant_retribution = PBField.new("revenant_retribution", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 61, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _revenant_retribution
		data[_revenant_retribution.tag] = service
		
		_revenant_salvation = PBField.new("revenant_salvation", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 62, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _revenant_salvation
		data[_revenant_salvation.tag] = service
		
		_thief_acrobatics = PBField.new("thief_acrobatics", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 63, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _thief_acrobatics
		data[_thief_acrobatics.tag] = service
		
		_thief_critical_strikes = PBField.new("thief_critical_strikes", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 64, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _thief_critical_strikes
		data[_thief_critical_strikes.tag] = service
		
		_thief_deadly_arts = PBField.new("thief_deadly_arts", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 65, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _thief_deadly_arts
		data[_thief_deadly_arts.tag] = service
		
		_thief_shadow_arts = PBField.new("thief_shadow_arts", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 66, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _thief_shadow_arts
		data[_thief_shadow_arts.tag] = service
		
		_thief_trickery = PBField.new("thief_trickery", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 67, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _thief_trickery
		data[_thief_trickery.tag] = service
		
		_warrior_arms = PBField.new("warrior_arms", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 68, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _warrior_arms
		data[_warrior_arms.tag] = service
		
		_warrior_defense = PBField.new("warrior_defense", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 69, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _warrior_defense
		data[_warrior_defense.tag] = service
		
		_warrior_discipline = PBField.new("warrior_discipline", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 70, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _warrior_discipline
		data[_warrior_discipline.tag] = service
		
		_warrior_strength = PBField.new("warrior_strength", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 71, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _warrior_strength
		data[_warrior_strength.tag] = service
		
		_warrior_tactics = PBField.new("warrior_tactics", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 72, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _warrior_tactics
		data[_warrior_tactics.tag] = service
		
	var data = {}
	
	var _elementalist_tempest: PBField
	func get_elementalist_tempest() -> bool:
		return _elementalist_tempest.value
	func clear_elementalist_tempest() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_elementalist_tempest.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_elementalist_tempest(value : bool) -> void:
		_elementalist_tempest.value = value
	
	var _engineer_scrapper: PBField
	func get_engineer_scrapper() -> bool:
		return _engineer_scrapper.value
	func clear_engineer_scrapper() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_engineer_scrapper.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_engineer_scrapper(value : bool) -> void:
		_engineer_scrapper.value = value
	
	var _guardian_dragonhunter: PBField
	func get_guardian_dragonhunter() -> bool:
		return _guardian_dragonhunter.value
	func clear_guardian_dragonhunter() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_guardian_dragonhunter.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_guardian_dragonhunter(value : bool) -> void:
		_guardian_dragonhunter.value = value
	
	var _mesmer_chronomancer: PBField
	func get_mesmer_chronomancer() -> bool:
		return _mesmer_chronomancer.value
	func clear_mesmer_chronomancer() -> void:
		data[4].state = PB_SERVICE_STATE.UNFILLED
		_mesmer_chronomancer.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_mesmer_chronomancer(value : bool) -> void:
		_mesmer_chronomancer.value = value
	
	var _necromancer_reaper: PBField
	func get_necromancer_reaper() -> bool:
		return _necromancer_reaper.value
	func clear_necromancer_reaper() -> void:
		data[5].state = PB_SERVICE_STATE.UNFILLED
		_necromancer_reaper.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_necromancer_reaper(value : bool) -> void:
		_necromancer_reaper.value = value
	
	var _ranger_druid: PBField
	func get_ranger_druid() -> bool:
		return _ranger_druid.value
	func clear_ranger_druid() -> void:
		data[6].state = PB_SERVICE_STATE.UNFILLED
		_ranger_druid.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_ranger_druid(value : bool) -> void:
		_ranger_druid.value = value
	
	var _revenant_herald: PBField
	func get_revenant_herald() -> bool:
		return _revenant_herald.value
	func clear_revenant_herald() -> void:
		data[7].state = PB_SERVICE_STATE.UNFILLED
		_revenant_herald.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_revenant_herald(value : bool) -> void:
		_revenant_herald.value = value
	
	var _thief_daredevil: PBField
	func get_thief_daredevil() -> bool:
		return _thief_daredevil.value
	func clear_thief_daredevil() -> void:
		data[8].state = PB_SERVICE_STATE.UNFILLED
		_thief_daredevil.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_thief_daredevil(value : bool) -> void:
		_thief_daredevil.value = value
	
	var _warrior_berserker: PBField
	func get_warrior_berserker() -> bool:
		return _warrior_berserker.value
	func clear_warrior_berserker() -> void:
		data[9].state = PB_SERVICE_STATE.UNFILLED
		_warrior_berserker.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_warrior_berserker(value : bool) -> void:
		_warrior_berserker.value = value
	
	var _elementalist_weaver: PBField
	func get_elementalist_weaver() -> bool:
		return _elementalist_weaver.value
	func clear_elementalist_weaver() -> void:
		data[10].state = PB_SERVICE_STATE.UNFILLED
		_elementalist_weaver.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_elementalist_weaver(value : bool) -> void:
		_elementalist_weaver.value = value
	
	var _engineer_holosmith: PBField
	func get_engineer_holosmith() -> bool:
		return _engineer_holosmith.value
	func clear_engineer_holosmith() -> void:
		data[11].state = PB_SERVICE_STATE.UNFILLED
		_engineer_holosmith.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_engineer_holosmith(value : bool) -> void:
		_engineer_holosmith.value = value
	
	var _guardian_firebrand: PBField
	func get_guardian_firebrand() -> bool:
		return _guardian_firebrand.value
	func clear_guardian_firebrand() -> void:
		data[12].state = PB_SERVICE_STATE.UNFILLED
		_guardian_firebrand.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_guardian_firebrand(value : bool) -> void:
		_guardian_firebrand.value = value
	
	var _mesmer_mirage: PBField
	func get_mesmer_mirage() -> bool:
		return _mesmer_mirage.value
	func clear_mesmer_mirage() -> void:
		data[13].state = PB_SERVICE_STATE.UNFILLED
		_mesmer_mirage.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_mesmer_mirage(value : bool) -> void:
		_mesmer_mirage.value = value
	
	var _necromancer_scourge: PBField
	func get_necromancer_scourge() -> bool:
		return _necromancer_scourge.value
	func clear_necromancer_scourge() -> void:
		data[14].state = PB_SERVICE_STATE.UNFILLED
		_necromancer_scourge.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_necromancer_scourge(value : bool) -> void:
		_necromancer_scourge.value = value
	
	var _ranger_soulbeast: PBField
	func get_ranger_soulbeast() -> bool:
		return _ranger_soulbeast.value
	func clear_ranger_soulbeast() -> void:
		data[15].state = PB_SERVICE_STATE.UNFILLED
		_ranger_soulbeast.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_ranger_soulbeast(value : bool) -> void:
		_ranger_soulbeast.value = value
	
	var _revenant_renegade: PBField
	func get_revenant_renegade() -> bool:
		return _revenant_renegade.value
	func clear_revenant_renegade() -> void:
		data[16].state = PB_SERVICE_STATE.UNFILLED
		_revenant_renegade.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_revenant_renegade(value : bool) -> void:
		_revenant_renegade.value = value
	
	var _thief_deadeye: PBField
	func get_thief_deadeye() -> bool:
		return _thief_deadeye.value
	func clear_thief_deadeye() -> void:
		data[17].state = PB_SERVICE_STATE.UNFILLED
		_thief_deadeye.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_thief_deadeye(value : bool) -> void:
		_thief_deadeye.value = value
	
	var _warrior_spellbreaker: PBField
	func get_warrior_spellbreaker() -> bool:
		return _warrior_spellbreaker.value
	func clear_warrior_spellbreaker() -> void:
		data[18].state = PB_SERVICE_STATE.UNFILLED
		_warrior_spellbreaker.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_warrior_spellbreaker(value : bool) -> void:
		_warrior_spellbreaker.value = value
	
	var _elementalist_catalyst: PBField
	func get_elementalist_catalyst() -> bool:
		return _elementalist_catalyst.value
	func clear_elementalist_catalyst() -> void:
		data[19].state = PB_SERVICE_STATE.UNFILLED
		_elementalist_catalyst.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_elementalist_catalyst(value : bool) -> void:
		_elementalist_catalyst.value = value
	
	var _engineer_mechanist: PBField
	func get_engineer_mechanist() -> bool:
		return _engineer_mechanist.value
	func clear_engineer_mechanist() -> void:
		data[20].state = PB_SERVICE_STATE.UNFILLED
		_engineer_mechanist.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_engineer_mechanist(value : bool) -> void:
		_engineer_mechanist.value = value
	
	var _guardian_willbender: PBField
	func get_guardian_willbender() -> bool:
		return _guardian_willbender.value
	func clear_guardian_willbender() -> void:
		data[21].state = PB_SERVICE_STATE.UNFILLED
		_guardian_willbender.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_guardian_willbender(value : bool) -> void:
		_guardian_willbender.value = value
	
	var _mesmer_virtuoso: PBField
	func get_mesmer_virtuoso() -> bool:
		return _mesmer_virtuoso.value
	func clear_mesmer_virtuoso() -> void:
		data[22].state = PB_SERVICE_STATE.UNFILLED
		_mesmer_virtuoso.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_mesmer_virtuoso(value : bool) -> void:
		_mesmer_virtuoso.value = value
	
	var _necromancer_harbinger: PBField
	func get_necromancer_harbinger() -> bool:
		return _necromancer_harbinger.value
	func clear_necromancer_harbinger() -> void:
		data[23].state = PB_SERVICE_STATE.UNFILLED
		_necromancer_harbinger.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_necromancer_harbinger(value : bool) -> void:
		_necromancer_harbinger.value = value
	
	var _ranger_untamed: PBField
	func get_ranger_untamed() -> bool:
		return _ranger_untamed.value
	func clear_ranger_untamed() -> void:
		data[24].state = PB_SERVICE_STATE.UNFILLED
		_ranger_untamed.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_ranger_untamed(value : bool) -> void:
		_ranger_untamed.value = value
	
	var _revenant_vindicator: PBField
	func get_revenant_vindicator() -> bool:
		return _revenant_vindicator.value
	func clear_revenant_vindicator() -> void:
		data[25].state = PB_SERVICE_STATE.UNFILLED
		_revenant_vindicator.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_revenant_vindicator(value : bool) -> void:
		_revenant_vindicator.value = value
	
	var _thief_specter: PBField
	func get_thief_specter() -> bool:
		return _thief_specter.value
	func clear_thief_specter() -> void:
		data[26].state = PB_SERVICE_STATE.UNFILLED
		_thief_specter.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_thief_specter(value : bool) -> void:
		_thief_specter.value = value
	
	var _warrior_bladesworn: PBField
	func get_warrior_bladesworn() -> bool:
		return _warrior_bladesworn.value
	func clear_warrior_bladesworn() -> void:
		data[27].state = PB_SERVICE_STATE.UNFILLED
		_warrior_bladesworn.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_warrior_bladesworn(value : bool) -> void:
		_warrior_bladesworn.value = value
	
	var _elementalist_air: PBField
	func get_elementalist_air() -> bool:
		return _elementalist_air.value
	func clear_elementalist_air() -> void:
		data[28].state = PB_SERVICE_STATE.UNFILLED
		_elementalist_air.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_elementalist_air(value : bool) -> void:
		_elementalist_air.value = value
	
	var _elementalist_arcane: PBField
	func get_elementalist_arcane() -> bool:
		return _elementalist_arcane.value
	func clear_elementalist_arcane() -> void:
		data[29].state = PB_SERVICE_STATE.UNFILLED
		_elementalist_arcane.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_elementalist_arcane(value : bool) -> void:
		_elementalist_arcane.value = value
	
	var _elementalist_earth: PBField
	func get_elementalist_earth() -> bool:
		return _elementalist_earth.value
	func clear_elementalist_earth() -> void:
		data[30].state = PB_SERVICE_STATE.UNFILLED
		_elementalist_earth.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_elementalist_earth(value : bool) -> void:
		_elementalist_earth.value = value
	
	var _elementalist_fire: PBField
	func get_elementalist_fire() -> bool:
		return _elementalist_fire.value
	func clear_elementalist_fire() -> void:
		data[31].state = PB_SERVICE_STATE.UNFILLED
		_elementalist_fire.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_elementalist_fire(value : bool) -> void:
		_elementalist_fire.value = value
	
	var _elementalist_water: PBField
	func get_elementalist_water() -> bool:
		return _elementalist_water.value
	func clear_elementalist_water() -> void:
		data[32].state = PB_SERVICE_STATE.UNFILLED
		_elementalist_water.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_elementalist_water(value : bool) -> void:
		_elementalist_water.value = value
	
	var _engineer_alchemy: PBField
	func get_engineer_alchemy() -> bool:
		return _engineer_alchemy.value
	func clear_engineer_alchemy() -> void:
		data[33].state = PB_SERVICE_STATE.UNFILLED
		_engineer_alchemy.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_engineer_alchemy(value : bool) -> void:
		_engineer_alchemy.value = value
	
	var _engineer_explosives: PBField
	func get_engineer_explosives() -> bool:
		return _engineer_explosives.value
	func clear_engineer_explosives() -> void:
		data[34].state = PB_SERVICE_STATE.UNFILLED
		_engineer_explosives.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_engineer_explosives(value : bool) -> void:
		_engineer_explosives.value = value
	
	var _engineer_firearms: PBField
	func get_engineer_firearms() -> bool:
		return _engineer_firearms.value
	func clear_engineer_firearms() -> void:
		data[35].state = PB_SERVICE_STATE.UNFILLED
		_engineer_firearms.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_engineer_firearms(value : bool) -> void:
		_engineer_firearms.value = value
	
	var _engineer_inventions: PBField
	func get_engineer_inventions() -> bool:
		return _engineer_inventions.value
	func clear_engineer_inventions() -> void:
		data[36].state = PB_SERVICE_STATE.UNFILLED
		_engineer_inventions.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_engineer_inventions(value : bool) -> void:
		_engineer_inventions.value = value
	
	var _engineer_tools: PBField
	func get_engineer_tools() -> bool:
		return _engineer_tools.value
	func clear_engineer_tools() -> void:
		data[37].state = PB_SERVICE_STATE.UNFILLED
		_engineer_tools.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_engineer_tools(value : bool) -> void:
		_engineer_tools.value = value
	
	var _guardian_honor: PBField
	func get_guardian_honor() -> bool:
		return _guardian_honor.value
	func clear_guardian_honor() -> void:
		data[38].state = PB_SERVICE_STATE.UNFILLED
		_guardian_honor.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_guardian_honor(value : bool) -> void:
		_guardian_honor.value = value
	
	var _guardian_radiance: PBField
	func get_guardian_radiance() -> bool:
		return _guardian_radiance.value
	func clear_guardian_radiance() -> void:
		data[39].state = PB_SERVICE_STATE.UNFILLED
		_guardian_radiance.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_guardian_radiance(value : bool) -> void:
		_guardian_radiance.value = value
	
	var _guardian_valor: PBField
	func get_guardian_valor() -> bool:
		return _guardian_valor.value
	func clear_guardian_valor() -> void:
		data[40].state = PB_SERVICE_STATE.UNFILLED
		_guardian_valor.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_guardian_valor(value : bool) -> void:
		_guardian_valor.value = value
	
	var _guardian_virtues: PBField
	func get_guardian_virtues() -> bool:
		return _guardian_virtues.value
	func clear_guardian_virtues() -> void:
		data[41].state = PB_SERVICE_STATE.UNFILLED
		_guardian_virtues.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_guardian_virtues(value : bool) -> void:
		_guardian_virtues.value = value
	
	var _guardian_zeal: PBField
	func get_guardian_zeal() -> bool:
		return _guardian_zeal.value
	func clear_guardian_zeal() -> void:
		data[42].state = PB_SERVICE_STATE.UNFILLED
		_guardian_zeal.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_guardian_zeal(value : bool) -> void:
		_guardian_zeal.value = value
	
	var _mesmer_chaos: PBField
	func get_mesmer_chaos() -> bool:
		return _mesmer_chaos.value
	func clear_mesmer_chaos() -> void:
		data[43].state = PB_SERVICE_STATE.UNFILLED
		_mesmer_chaos.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_mesmer_chaos(value : bool) -> void:
		_mesmer_chaos.value = value
	
	var _mesmer_domination: PBField
	func get_mesmer_domination() -> bool:
		return _mesmer_domination.value
	func clear_mesmer_domination() -> void:
		data[44].state = PB_SERVICE_STATE.UNFILLED
		_mesmer_domination.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_mesmer_domination(value : bool) -> void:
		_mesmer_domination.value = value
	
	var _mesmer_dueling: PBField
	func get_mesmer_dueling() -> bool:
		return _mesmer_dueling.value
	func clear_mesmer_dueling() -> void:
		data[45].state = PB_SERVICE_STATE.UNFILLED
		_mesmer_dueling.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_mesmer_dueling(value : bool) -> void:
		_mesmer_dueling.value = value
	
	var _mesmer_illusions: PBField
	func get_mesmer_illusions() -> bool:
		return _mesmer_illusions.value
	func clear_mesmer_illusions() -> void:
		data[46].state = PB_SERVICE_STATE.UNFILLED
		_mesmer_illusions.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_mesmer_illusions(value : bool) -> void:
		_mesmer_illusions.value = value
	
	var _mesmer_inspiration: PBField
	func get_mesmer_inspiration() -> bool:
		return _mesmer_inspiration.value
	func clear_mesmer_inspiration() -> void:
		data[47].state = PB_SERVICE_STATE.UNFILLED
		_mesmer_inspiration.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_mesmer_inspiration(value : bool) -> void:
		_mesmer_inspiration.value = value
	
	var _necromancer_blood_magic: PBField
	func get_necromancer_blood_magic() -> bool:
		return _necromancer_blood_magic.value
	func clear_necromancer_blood_magic() -> void:
		data[48].state = PB_SERVICE_STATE.UNFILLED
		_necromancer_blood_magic.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_necromancer_blood_magic(value : bool) -> void:
		_necromancer_blood_magic.value = value
	
	var _necromancer_curses: PBField
	func get_necromancer_curses() -> bool:
		return _necromancer_curses.value
	func clear_necromancer_curses() -> void:
		data[49].state = PB_SERVICE_STATE.UNFILLED
		_necromancer_curses.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_necromancer_curses(value : bool) -> void:
		_necromancer_curses.value = value
	
	var _necromancer_death_magic: PBField
	func get_necromancer_death_magic() -> bool:
		return _necromancer_death_magic.value
	func clear_necromancer_death_magic() -> void:
		data[50].state = PB_SERVICE_STATE.UNFILLED
		_necromancer_death_magic.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_necromancer_death_magic(value : bool) -> void:
		_necromancer_death_magic.value = value
	
	var _necromancer_soul_reaping: PBField
	func get_necromancer_soul_reaping() -> bool:
		return _necromancer_soul_reaping.value
	func clear_necromancer_soul_reaping() -> void:
		data[51].state = PB_SERVICE_STATE.UNFILLED
		_necromancer_soul_reaping.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_necromancer_soul_reaping(value : bool) -> void:
		_necromancer_soul_reaping.value = value
	
	var _necromancer_spite: PBField
	func get_necromancer_spite() -> bool:
		return _necromancer_spite.value
	func clear_necromancer_spite() -> void:
		data[52].state = PB_SERVICE_STATE.UNFILLED
		_necromancer_spite.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_necromancer_spite(value : bool) -> void:
		_necromancer_spite.value = value
	
	var _ranger_beastmastery: PBField
	func get_ranger_beastmastery() -> bool:
		return _ranger_beastmastery.value
	func clear_ranger_beastmastery() -> void:
		data[53].state = PB_SERVICE_STATE.UNFILLED
		_ranger_beastmastery.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_ranger_beastmastery(value : bool) -> void:
		_ranger_beastmastery.value = value
	
	var _ranger_marksmanship: PBField
	func get_ranger_marksmanship() -> bool:
		return _ranger_marksmanship.value
	func clear_ranger_marksmanship() -> void:
		data[54].state = PB_SERVICE_STATE.UNFILLED
		_ranger_marksmanship.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_ranger_marksmanship(value : bool) -> void:
		_ranger_marksmanship.value = value
	
	var _ranger_nature_magic: PBField
	func get_ranger_nature_magic() -> bool:
		return _ranger_nature_magic.value
	func clear_ranger_nature_magic() -> void:
		data[55].state = PB_SERVICE_STATE.UNFILLED
		_ranger_nature_magic.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_ranger_nature_magic(value : bool) -> void:
		_ranger_nature_magic.value = value
	
	var _ranger_skirmishing: PBField
	func get_ranger_skirmishing() -> bool:
		return _ranger_skirmishing.value
	func clear_ranger_skirmishing() -> void:
		data[56].state = PB_SERVICE_STATE.UNFILLED
		_ranger_skirmishing.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_ranger_skirmishing(value : bool) -> void:
		_ranger_skirmishing.value = value
	
	var _ranger_wilderness_survival: PBField
	func get_ranger_wilderness_survival() -> bool:
		return _ranger_wilderness_survival.value
	func clear_ranger_wilderness_survival() -> void:
		data[57].state = PB_SERVICE_STATE.UNFILLED
		_ranger_wilderness_survival.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_ranger_wilderness_survival(value : bool) -> void:
		_ranger_wilderness_survival.value = value
	
	var _revenant_corruption: PBField
	func get_revenant_corruption() -> bool:
		return _revenant_corruption.value
	func clear_revenant_corruption() -> void:
		data[58].state = PB_SERVICE_STATE.UNFILLED
		_revenant_corruption.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_revenant_corruption(value : bool) -> void:
		_revenant_corruption.value = value
	
	var _revenant_devastation: PBField
	func get_revenant_devastation() -> bool:
		return _revenant_devastation.value
	func clear_revenant_devastation() -> void:
		data[59].state = PB_SERVICE_STATE.UNFILLED
		_revenant_devastation.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_revenant_devastation(value : bool) -> void:
		_revenant_devastation.value = value
	
	var _revenant_invocation: PBField
	func get_revenant_invocation() -> bool:
		return _revenant_invocation.value
	func clear_revenant_invocation() -> void:
		data[60].state = PB_SERVICE_STATE.UNFILLED
		_revenant_invocation.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_revenant_invocation(value : bool) -> void:
		_revenant_invocation.value = value
	
	var _revenant_retribution: PBField
	func get_revenant_retribution() -> bool:
		return _revenant_retribution.value
	func clear_revenant_retribution() -> void:
		data[61].state = PB_SERVICE_STATE.UNFILLED
		_revenant_retribution.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_revenant_retribution(value : bool) -> void:
		_revenant_retribution.value = value
	
	var _revenant_salvation: PBField
	func get_revenant_salvation() -> bool:
		return _revenant_salvation.value
	func clear_revenant_salvation() -> void:
		data[62].state = PB_SERVICE_STATE.UNFILLED
		_revenant_salvation.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_revenant_salvation(value : bool) -> void:
		_revenant_salvation.value = value
	
	var _thief_acrobatics: PBField
	func get_thief_acrobatics() -> bool:
		return _thief_acrobatics.value
	func clear_thief_acrobatics() -> void:
		data[63].state = PB_SERVICE_STATE.UNFILLED
		_thief_acrobatics.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_thief_acrobatics(value : bool) -> void:
		_thief_acrobatics.value = value
	
	var _thief_critical_strikes: PBField
	func get_thief_critical_strikes() -> bool:
		return _thief_critical_strikes.value
	func clear_thief_critical_strikes() -> void:
		data[64].state = PB_SERVICE_STATE.UNFILLED
		_thief_critical_strikes.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_thief_critical_strikes(value : bool) -> void:
		_thief_critical_strikes.value = value
	
	var _thief_deadly_arts: PBField
	func get_thief_deadly_arts() -> bool:
		return _thief_deadly_arts.value
	func clear_thief_deadly_arts() -> void:
		data[65].state = PB_SERVICE_STATE.UNFILLED
		_thief_deadly_arts.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_thief_deadly_arts(value : bool) -> void:
		_thief_deadly_arts.value = value
	
	var _thief_shadow_arts: PBField
	func get_thief_shadow_arts() -> bool:
		return _thief_shadow_arts.value
	func clear_thief_shadow_arts() -> void:
		data[66].state = PB_SERVICE_STATE.UNFILLED
		_thief_shadow_arts.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_thief_shadow_arts(value : bool) -> void:
		_thief_shadow_arts.value = value
	
	var _thief_trickery: PBField
	func get_thief_trickery() -> bool:
		return _thief_trickery.value
	func clear_thief_trickery() -> void:
		data[67].state = PB_SERVICE_STATE.UNFILLED
		_thief_trickery.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_thief_trickery(value : bool) -> void:
		_thief_trickery.value = value
	
	var _warrior_arms: PBField
	func get_warrior_arms() -> bool:
		return _warrior_arms.value
	func clear_warrior_arms() -> void:
		data[68].state = PB_SERVICE_STATE.UNFILLED
		_warrior_arms.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_warrior_arms(value : bool) -> void:
		_warrior_arms.value = value
	
	var _warrior_defense: PBField
	func get_warrior_defense() -> bool:
		return _warrior_defense.value
	func clear_warrior_defense() -> void:
		data[69].state = PB_SERVICE_STATE.UNFILLED
		_warrior_defense.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_warrior_defense(value : bool) -> void:
		_warrior_defense.value = value
	
	var _warrior_discipline: PBField
	func get_warrior_discipline() -> bool:
		return _warrior_discipline.value
	func clear_warrior_discipline() -> void:
		data[70].state = PB_SERVICE_STATE.UNFILLED
		_warrior_discipline.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_warrior_discipline(value : bool) -> void:
		_warrior_discipline.value = value
	
	var _warrior_strength: PBField
	func get_warrior_strength() -> bool:
		return _warrior_strength.value
	func clear_warrior_strength() -> void:
		data[71].state = PB_SERVICE_STATE.UNFILLED
		_warrior_strength.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_warrior_strength(value : bool) -> void:
		_warrior_strength.value = value
	
	var _warrior_tactics: PBField
	func get_warrior_tactics() -> bool:
		return _warrior_tactics.value
	func clear_warrior_tactics() -> void:
		data[72].state = PB_SERVICE_STATE.UNFILLED
		_warrior_tactics.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_warrior_tactics(value : bool) -> void:
		_warrior_tactics.value = value
	
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
class SpeciesFilter:
	func _init():
		var service
		
		_asura = PBField.new("asura", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 1, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _asura
		data[_asura.tag] = service
		
		_charr = PBField.new("charr", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _charr
		data[_charr.tag] = service
		
		_human = PBField.new("human", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 3, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _human
		data[_human.tag] = service
		
		_norn = PBField.new("norn", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 4, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _norn
		data[_norn.tag] = service
		
		_sylvari = PBField.new("sylvari", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 5, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _sylvari
		data[_sylvari.tag] = service
		
	var data = {}
	
	var _asura: PBField
	func get_asura() -> bool:
		return _asura.value
	func clear_asura() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_asura.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_asura(value : bool) -> void:
		_asura.value = value
	
	var _charr: PBField
	func get_charr() -> bool:
		return _charr.value
	func clear_charr() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_charr.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_charr(value : bool) -> void:
		_charr.value = value
	
	var _human: PBField
	func get_human() -> bool:
		return _human.value
	func clear_human() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_human.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_human(value : bool) -> void:
		_human.value = value
	
	var _norn: PBField
	func get_norn() -> bool:
		return _norn.value
	func clear_norn() -> void:
		data[4].state = PB_SERVICE_STATE.UNFILLED
		_norn.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_norn(value : bool) -> void:
		_norn.value = value
	
	var _sylvari: PBField
	func get_sylvari() -> bool:
		return _sylvari.value
	func clear_sylvari() -> void:
		data[5].state = PB_SERVICE_STATE.UNFILLED
		_sylvari.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_sylvari(value : bool) -> void:
		_sylvari.value = value
	
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
class TrailData:
	func _init():
		var service
		
		_points_x = PBField.new("points_x", PB_DATA_TYPE.FLOAT, PB_RULE.REPEATED, 2, true, [])
		service = PBServiceField.new()
		service.field = _points_x
		data[_points_x.tag] = service
		
		_points_y = PBField.new("points_y", PB_DATA_TYPE.FLOAT, PB_RULE.REPEATED, 3, true, [])
		service = PBServiceField.new()
		service.field = _points_y
		data[_points_y.tag] = service
		
		_points_z = PBField.new("points_z", PB_DATA_TYPE.FLOAT, PB_RULE.REPEATED, 4, true, [])
		service = PBServiceField.new()
		service.field = _points_z
		data[_points_z.tag] = service
		
	var data = {}
	
	var _points_x: PBField
	func get_points_x() -> Array:
		return _points_x.value
	func clear_points_x() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_points_x.value = []
	func add_points_x(value : float) -> void:
		_points_x.value.append(value)
	
	var _points_y: PBField
	func get_points_y() -> Array:
		return _points_y.value
	func clear_points_y() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_points_y.value = []
	func add_points_y(value : float) -> void:
		_points_y.value.append(value)
	
	var _points_z: PBField
	func get_points_z() -> Array:
		return _points_z.value
	func clear_points_z() -> void:
		data[4].state = PB_SERVICE_STATE.UNFILLED
		_points_z.value = []
	func add_points_z(value : float) -> void:
		_points_z.value.append(value)
	
	func to_string() -> String:
		return PBPacker.message_to_string(data)
		
	func to_bytes() -> PoolByteArray:
		return PBPacker.pack_message(data)
		
	func from_bytes(bytes : PoolByteArray, offset : int = 0, limit : int = -1) -> int:
		var cur_limit = bytes.size()
		if limit != -1:
			cur_limit = limit
		var result = PBPacker.unpack_message(data, bytes, offset, cur_limit)
		if result == cur_limit:
			if PBPacker.check_required(data):
				if limit == -1:
					return PB_ERR.NO_ERRORS
			else:
				return PB_ERR.REQUIRED_FIELDS
		elif limit == -1 && result > 0:
			return PB_ERR.PARSE_INCOMPLETE
		return result
	
################ USER DATA END #################
