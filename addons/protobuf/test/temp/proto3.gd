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


class Test0:
	func _init():
		var service
		
	var data = {}
	
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
	
enum Enum0 {
	NULL = 0,
	ONE = 1,
	TWO = 2,
	THREE = 3,
	FOUR = 4
}

class Test1:
	func _init():
		var service
		
		_f_double = PBField.new("f_double", PB_DATA_TYPE.DOUBLE, PB_RULE.OPTIONAL, 1, true, DEFAULT_VALUES_3[PB_DATA_TYPE.DOUBLE])
		service = PBServiceField.new()
		service.field = _f_double
		data[_f_double.tag] = service
		
		_f_float = PBField.new("f_float", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _f_float
		data[_f_float.tag] = service
		
		_f_int32 = PBField.new("f_int32", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 3, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
		service = PBServiceField.new()
		service.field = _f_int32
		data[_f_int32.tag] = service
		
		_f_int64 = PBField.new("f_int64", PB_DATA_TYPE.INT64, PB_RULE.OPTIONAL, 4, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT64])
		service = PBServiceField.new()
		service.field = _f_int64
		data[_f_int64.tag] = service
		
		_f_uint32 = PBField.new("f_uint32", PB_DATA_TYPE.UINT32, PB_RULE.OPTIONAL, 5, true, DEFAULT_VALUES_3[PB_DATA_TYPE.UINT32])
		service = PBServiceField.new()
		service.field = _f_uint32
		data[_f_uint32.tag] = service
		
		_f_uint64 = PBField.new("f_uint64", PB_DATA_TYPE.UINT64, PB_RULE.OPTIONAL, 6, true, DEFAULT_VALUES_3[PB_DATA_TYPE.UINT64])
		service = PBServiceField.new()
		service.field = _f_uint64
		data[_f_uint64.tag] = service
		
		_f_sint32 = PBField.new("f_sint32", PB_DATA_TYPE.SINT32, PB_RULE.OPTIONAL, 7, true, DEFAULT_VALUES_3[PB_DATA_TYPE.SINT32])
		service = PBServiceField.new()
		service.field = _f_sint32
		data[_f_sint32.tag] = service
		
		_f_sint64 = PBField.new("f_sint64", PB_DATA_TYPE.SINT64, PB_RULE.OPTIONAL, 8, true, DEFAULT_VALUES_3[PB_DATA_TYPE.SINT64])
		service = PBServiceField.new()
		service.field = _f_sint64
		data[_f_sint64.tag] = service
		
		_f_fixed32 = PBField.new("f_fixed32", PB_DATA_TYPE.FIXED32, PB_RULE.OPTIONAL, 9, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FIXED32])
		service = PBServiceField.new()
		service.field = _f_fixed32
		data[_f_fixed32.tag] = service
		
		_f_fixed64 = PBField.new("f_fixed64", PB_DATA_TYPE.FIXED64, PB_RULE.OPTIONAL, 10, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FIXED64])
		service = PBServiceField.new()
		service.field = _f_fixed64
		data[_f_fixed64.tag] = service
		
		_f_sfixed32 = PBField.new("f_sfixed32", PB_DATA_TYPE.SFIXED32, PB_RULE.OPTIONAL, 11, true, DEFAULT_VALUES_3[PB_DATA_TYPE.SFIXED32])
		service = PBServiceField.new()
		service.field = _f_sfixed32
		data[_f_sfixed32.tag] = service
		
		_f_sfixed64 = PBField.new("f_sfixed64", PB_DATA_TYPE.SFIXED64, PB_RULE.OPTIONAL, 12, true, DEFAULT_VALUES_3[PB_DATA_TYPE.SFIXED64])
		service = PBServiceField.new()
		service.field = _f_sfixed64
		data[_f_sfixed64.tag] = service
		
		_f_bool = PBField.new("f_bool", PB_DATA_TYPE.BOOL, PB_RULE.OPTIONAL, 13, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL])
		service = PBServiceField.new()
		service.field = _f_bool
		data[_f_bool.tag] = service
		
		_f_string = PBField.new("f_string", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 14, true, DEFAULT_VALUES_3[PB_DATA_TYPE.STRING])
		service = PBServiceField.new()
		service.field = _f_string
		data[_f_string.tag] = service
		
		_f_bytes = PBField.new("f_bytes", PB_DATA_TYPE.BYTES, PB_RULE.OPTIONAL, 15, true, DEFAULT_VALUES_3[PB_DATA_TYPE.BYTES])
		service = PBServiceField.new()
		service.field = _f_bytes
		data[_f_bytes.tag] = service
		
		_f_map = PBField.new("f_map", PB_DATA_TYPE.MAP, PB_RULE.REPEATED, 16, true, [])
		service = PBServiceField.new()
		service.field = _f_map
		service.func_ref = funcref(self, "add_empty_f_map")
		data[_f_map.tag] = service
		
		_f_oneof_f1 = PBField.new("f_oneof_f1", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 17, true, DEFAULT_VALUES_3[PB_DATA_TYPE.STRING])
		service = PBServiceField.new()
		service.field = _f_oneof_f1
		data[_f_oneof_f1.tag] = service
		
		_f_oneof_f2 = PBField.new("f_oneof_f2", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 18, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
		service = PBServiceField.new()
		service.field = _f_oneof_f2
		data[_f_oneof_f2.tag] = service
		
		_f_empty_out = PBField.new("f_empty_out", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 19, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _f_empty_out
		service.func_ref = funcref(self, "new_f_empty_out")
		data[_f_empty_out.tag] = service
		
		_f_enum_out = PBField.new("f_enum_out", PB_DATA_TYPE.ENUM, PB_RULE.OPTIONAL, 20, true, DEFAULT_VALUES_3[PB_DATA_TYPE.ENUM])
		service = PBServiceField.new()
		service.field = _f_enum_out
		data[_f_enum_out.tag] = service
		
		_f_empty_inner = PBField.new("f_empty_inner", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 21, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _f_empty_inner
		service.func_ref = funcref(self, "new_f_empty_inner")
		data[_f_empty_inner.tag] = service
		
		_f_enum_inner = PBField.new("f_enum_inner", PB_DATA_TYPE.ENUM, PB_RULE.OPTIONAL, 22, true, DEFAULT_VALUES_3[PB_DATA_TYPE.ENUM])
		service = PBServiceField.new()
		service.field = _f_enum_inner
		data[_f_enum_inner.tag] = service
		
		_rf_double = PBField.new("rf_double", PB_DATA_TYPE.DOUBLE, PB_RULE.REPEATED, 23, true, [])
		service = PBServiceField.new()
		service.field = _rf_double
		data[_rf_double.tag] = service
		
		_rf_float = PBField.new("rf_float", PB_DATA_TYPE.FLOAT, PB_RULE.REPEATED, 24, true, [])
		service = PBServiceField.new()
		service.field = _rf_float
		data[_rf_float.tag] = service
		
		_rf_int32 = PBField.new("rf_int32", PB_DATA_TYPE.INT32, PB_RULE.REPEATED, 25, true, [])
		service = PBServiceField.new()
		service.field = _rf_int32
		data[_rf_int32.tag] = service
		
		_rf_int64 = PBField.new("rf_int64", PB_DATA_TYPE.INT64, PB_RULE.REPEATED, 26, true, [])
		service = PBServiceField.new()
		service.field = _rf_int64
		data[_rf_int64.tag] = service
		
		_rf_uint32 = PBField.new("rf_uint32", PB_DATA_TYPE.UINT32, PB_RULE.REPEATED, 27, true, [])
		service = PBServiceField.new()
		service.field = _rf_uint32
		data[_rf_uint32.tag] = service
		
		_rf_uint64 = PBField.new("rf_uint64", PB_DATA_TYPE.UINT64, PB_RULE.REPEATED, 28, true, [])
		service = PBServiceField.new()
		service.field = _rf_uint64
		data[_rf_uint64.tag] = service
		
		_rf_sint32 = PBField.new("rf_sint32", PB_DATA_TYPE.SINT32, PB_RULE.REPEATED, 29, true, [])
		service = PBServiceField.new()
		service.field = _rf_sint32
		data[_rf_sint32.tag] = service
		
		_rf_sint64 = PBField.new("rf_sint64", PB_DATA_TYPE.SINT64, PB_RULE.REPEATED, 30, true, [])
		service = PBServiceField.new()
		service.field = _rf_sint64
		data[_rf_sint64.tag] = service
		
		_rf_fixed32 = PBField.new("rf_fixed32", PB_DATA_TYPE.FIXED32, PB_RULE.REPEATED, 31, true, [])
		service = PBServiceField.new()
		service.field = _rf_fixed32
		data[_rf_fixed32.tag] = service
		
		_rf_fixed64 = PBField.new("rf_fixed64", PB_DATA_TYPE.FIXED64, PB_RULE.REPEATED, 32, true, [])
		service = PBServiceField.new()
		service.field = _rf_fixed64
		data[_rf_fixed64.tag] = service
		
		_rf_sfixed32 = PBField.new("rf_sfixed32", PB_DATA_TYPE.SFIXED32, PB_RULE.REPEATED, 33, true, [])
		service = PBServiceField.new()
		service.field = _rf_sfixed32
		data[_rf_sfixed32.tag] = service
		
		_rf_sfixed64 = PBField.new("rf_sfixed64", PB_DATA_TYPE.SFIXED64, PB_RULE.REPEATED, 34, true, [])
		service = PBServiceField.new()
		service.field = _rf_sfixed64
		data[_rf_sfixed64.tag] = service
		
		_rf_bool = PBField.new("rf_bool", PB_DATA_TYPE.BOOL, PB_RULE.REPEATED, 35, true, [])
		service = PBServiceField.new()
		service.field = _rf_bool
		data[_rf_bool.tag] = service
		
		_rf_string = PBField.new("rf_string", PB_DATA_TYPE.STRING, PB_RULE.REPEATED, 36, true, [])
		service = PBServiceField.new()
		service.field = _rf_string
		data[_rf_string.tag] = service
		
		_rf_bytes = PBField.new("rf_bytes", PB_DATA_TYPE.BYTES, PB_RULE.REPEATED, 37, true, [])
		service = PBServiceField.new()
		service.field = _rf_bytes
		data[_rf_bytes.tag] = service
		
		_rf_empty_out = PBField.new("rf_empty_out", PB_DATA_TYPE.MESSAGE, PB_RULE.REPEATED, 38, true, [])
		service = PBServiceField.new()
		service.field = _rf_empty_out
		service.func_ref = funcref(self, "add_rf_empty_out")
		data[_rf_empty_out.tag] = service
		
		_rf_enum_out = PBField.new("rf_enum_out", PB_DATA_TYPE.ENUM, PB_RULE.REPEATED, 39, true, [])
		service = PBServiceField.new()
		service.field = _rf_enum_out
		data[_rf_enum_out.tag] = service
		
		_rf_empty_inner = PBField.new("rf_empty_inner", PB_DATA_TYPE.MESSAGE, PB_RULE.REPEATED, 40, true, [])
		service = PBServiceField.new()
		service.field = _rf_empty_inner
		service.func_ref = funcref(self, "add_rf_empty_inner")
		data[_rf_empty_inner.tag] = service
		
		_rf_enum_inner = PBField.new("rf_enum_inner", PB_DATA_TYPE.ENUM, PB_RULE.REPEATED, 41, true, [])
		service = PBServiceField.new()
		service.field = _rf_enum_inner
		data[_rf_enum_inner.tag] = service
		
		_rfu_double = PBField.new("rfu_double", PB_DATA_TYPE.DOUBLE, PB_RULE.REPEATED, 42, false, [])
		service = PBServiceField.new()
		service.field = _rfu_double
		data[_rfu_double.tag] = service
		
		_rfu_float = PBField.new("rfu_float", PB_DATA_TYPE.FLOAT, PB_RULE.REPEATED, 43, false, [])
		service = PBServiceField.new()
		service.field = _rfu_float
		data[_rfu_float.tag] = service
		
		_rfu_int32f = PBField.new("rfu_int32f", PB_DATA_TYPE.INT32, PB_RULE.REPEATED, 44, false, [])
		service = PBServiceField.new()
		service.field = _rfu_int32f
		data[_rfu_int32f.tag] = service
		
		_rfu_int64f = PBField.new("rfu_int64f", PB_DATA_TYPE.INT64, PB_RULE.REPEATED, 45, false, [])
		service = PBServiceField.new()
		service.field = _rfu_int64f
		data[_rfu_int64f.tag] = service
		
		_rfu_uint32 = PBField.new("rfu_uint32", PB_DATA_TYPE.UINT32, PB_RULE.REPEATED, 46, false, [])
		service = PBServiceField.new()
		service.field = _rfu_uint32
		data[_rfu_uint32.tag] = service
		
		_rfu_uint64 = PBField.new("rfu_uint64", PB_DATA_TYPE.UINT64, PB_RULE.REPEATED, 47, false, [])
		service = PBServiceField.new()
		service.field = _rfu_uint64
		data[_rfu_uint64.tag] = service
		
		_rfu_sint32 = PBField.new("rfu_sint32", PB_DATA_TYPE.SINT32, PB_RULE.REPEATED, 48, false, [])
		service = PBServiceField.new()
		service.field = _rfu_sint32
		data[_rfu_sint32.tag] = service
		
		_rfu_sint64 = PBField.new("rfu_sint64", PB_DATA_TYPE.SINT64, PB_RULE.REPEATED, 49, false, [])
		service = PBServiceField.new()
		service.field = _rfu_sint64
		data[_rfu_sint64.tag] = service
		
		_rfu_fixed32 = PBField.new("rfu_fixed32", PB_DATA_TYPE.FIXED32, PB_RULE.REPEATED, 50, false, [])
		service = PBServiceField.new()
		service.field = _rfu_fixed32
		data[_rfu_fixed32.tag] = service
		
		_rfu_fixed64 = PBField.new("rfu_fixed64", PB_DATA_TYPE.FIXED64, PB_RULE.REPEATED, 51, false, [])
		service = PBServiceField.new()
		service.field = _rfu_fixed64
		data[_rfu_fixed64.tag] = service
		
		_rfu_sfixed32 = PBField.new("rfu_sfixed32", PB_DATA_TYPE.SFIXED32, PB_RULE.REPEATED, 52, false, [])
		service = PBServiceField.new()
		service.field = _rfu_sfixed32
		data[_rfu_sfixed32.tag] = service
		
		_rfu_sfixed64 = PBField.new("rfu_sfixed64", PB_DATA_TYPE.SFIXED64, PB_RULE.REPEATED, 53, false, [])
		service = PBServiceField.new()
		service.field = _rfu_sfixed64
		data[_rfu_sfixed64.tag] = service
		
		_rfu_bool = PBField.new("rfu_bool", PB_DATA_TYPE.BOOL, PB_RULE.REPEATED, 54, false, [])
		service = PBServiceField.new()
		service.field = _rfu_bool
		data[_rfu_bool.tag] = service
		
		_rf_inner = PBField.new("rf_inner", PB_DATA_TYPE.MESSAGE, PB_RULE.REPEATED, 55, true, [])
		service = PBServiceField.new()
		service.field = _rf_inner
		service.func_ref = funcref(self, "add_rf_inner")
		data[_rf_inner.tag] = service
		
	var data = {}
	
	var _f_double: PBField
	func get_f_double() -> float:
		return _f_double.value
	func clear_f_double() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_f_double.value = DEFAULT_VALUES_3[PB_DATA_TYPE.DOUBLE]
	func set_f_double(value : float) -> void:
		_f_double.value = value
	
	var _f_float: PBField
	func get_f_float() -> float:
		return _f_float.value
	func clear_f_float() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_f_float.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_f_float(value : float) -> void:
		_f_float.value = value
	
	var _f_int32: PBField
	func get_f_int32() -> int:
		return _f_int32.value
	func clear_f_int32() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_f_int32.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
	func set_f_int32(value : int) -> void:
		_f_int32.value = value
	
	var _f_int64: PBField
	func get_f_int64() -> int:
		return _f_int64.value
	func clear_f_int64() -> void:
		data[4].state = PB_SERVICE_STATE.UNFILLED
		_f_int64.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT64]
	func set_f_int64(value : int) -> void:
		_f_int64.value = value
	
	var _f_uint32: PBField
	func get_f_uint32() -> int:
		return _f_uint32.value
	func clear_f_uint32() -> void:
		data[5].state = PB_SERVICE_STATE.UNFILLED
		_f_uint32.value = DEFAULT_VALUES_3[PB_DATA_TYPE.UINT32]
	func set_f_uint32(value : int) -> void:
		_f_uint32.value = value
	
	var _f_uint64: PBField
	func get_f_uint64() -> int:
		return _f_uint64.value
	func clear_f_uint64() -> void:
		data[6].state = PB_SERVICE_STATE.UNFILLED
		_f_uint64.value = DEFAULT_VALUES_3[PB_DATA_TYPE.UINT64]
	func set_f_uint64(value : int) -> void:
		_f_uint64.value = value
	
	var _f_sint32: PBField
	func get_f_sint32() -> int:
		return _f_sint32.value
	func clear_f_sint32() -> void:
		data[7].state = PB_SERVICE_STATE.UNFILLED
		_f_sint32.value = DEFAULT_VALUES_3[PB_DATA_TYPE.SINT32]
	func set_f_sint32(value : int) -> void:
		_f_sint32.value = value
	
	var _f_sint64: PBField
	func get_f_sint64() -> int:
		return _f_sint64.value
	func clear_f_sint64() -> void:
		data[8].state = PB_SERVICE_STATE.UNFILLED
		_f_sint64.value = DEFAULT_VALUES_3[PB_DATA_TYPE.SINT64]
	func set_f_sint64(value : int) -> void:
		_f_sint64.value = value
	
	var _f_fixed32: PBField
	func get_f_fixed32() -> int:
		return _f_fixed32.value
	func clear_f_fixed32() -> void:
		data[9].state = PB_SERVICE_STATE.UNFILLED
		_f_fixed32.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FIXED32]
	func set_f_fixed32(value : int) -> void:
		_f_fixed32.value = value
	
	var _f_fixed64: PBField
	func get_f_fixed64() -> int:
		return _f_fixed64.value
	func clear_f_fixed64() -> void:
		data[10].state = PB_SERVICE_STATE.UNFILLED
		_f_fixed64.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FIXED64]
	func set_f_fixed64(value : int) -> void:
		_f_fixed64.value = value
	
	var _f_sfixed32: PBField
	func get_f_sfixed32() -> int:
		return _f_sfixed32.value
	func clear_f_sfixed32() -> void:
		data[11].state = PB_SERVICE_STATE.UNFILLED
		_f_sfixed32.value = DEFAULT_VALUES_3[PB_DATA_TYPE.SFIXED32]
	func set_f_sfixed32(value : int) -> void:
		_f_sfixed32.value = value
	
	var _f_sfixed64: PBField
	func get_f_sfixed64() -> int:
		return _f_sfixed64.value
	func clear_f_sfixed64() -> void:
		data[12].state = PB_SERVICE_STATE.UNFILLED
		_f_sfixed64.value = DEFAULT_VALUES_3[PB_DATA_TYPE.SFIXED64]
	func set_f_sfixed64(value : int) -> void:
		_f_sfixed64.value = value
	
	var _f_bool: PBField
	func get_f_bool() -> bool:
		return _f_bool.value
	func clear_f_bool() -> void:
		data[13].state = PB_SERVICE_STATE.UNFILLED
		_f_bool.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BOOL]
	func set_f_bool(value : bool) -> void:
		_f_bool.value = value
	
	var _f_string: PBField
	func get_f_string() -> String:
		return _f_string.value
	func clear_f_string() -> void:
		data[14].state = PB_SERVICE_STATE.UNFILLED
		_f_string.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
	func set_f_string(value : String) -> void:
		_f_string.value = value
	
	var _f_bytes: PBField
	func get_f_bytes() -> PoolByteArray:
		return _f_bytes.value
	func clear_f_bytes() -> void:
		data[15].state = PB_SERVICE_STATE.UNFILLED
		_f_bytes.value = DEFAULT_VALUES_3[PB_DATA_TYPE.BYTES]
	func set_f_bytes(value : PoolByteArray) -> void:
		_f_bytes.value = value
	
	var _f_map: PBField
	func get_raw_f_map():
		return _f_map.value
	func get_f_map():
		return PBPacker.construct_map(_f_map.value)
	func clear_f_map():
		data[16].state = PB_SERVICE_STATE.UNFILLED
		_f_map.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MAP]
	func add_empty_f_map() -> int:
		var element = Test1.map_type_f_map.new()
		_f_map.value.append(element)
		return element
	func add_f_map(a_key, a_value) -> void:
		var idx = -1
		for i in range(_f_map.value.size()):
			if _f_map.value[i].get_key() == a_key:
				idx = i
				break
		var element = Test1.map_type_f_map.new()
		element.set_key(a_key)
		element.set_value(a_value)
		if idx != -1:
			_f_map.value[idx] = element
		else:
			_f_map.value.append(element)
	
	var _f_oneof_f1: PBField
	func has_f_oneof_f1() -> bool:
		return data[17].state == PB_SERVICE_STATE.FILLED
	func get_f_oneof_f1() -> String:
		return _f_oneof_f1.value
	func clear_f_oneof_f1() -> void:
		data[17].state = PB_SERVICE_STATE.UNFILLED
		_f_oneof_f1.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
	func set_f_oneof_f1(value : String) -> void:
		data[17].state = PB_SERVICE_STATE.FILLED
		_f_oneof_f2.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
		data[18].state = PB_SERVICE_STATE.UNFILLED
		_f_oneof_f1.value = value
	
	var _f_oneof_f2: PBField
	func has_f_oneof_f2() -> bool:
		return data[18].state == PB_SERVICE_STATE.FILLED
	func get_f_oneof_f2() -> int:
		return _f_oneof_f2.value
	func clear_f_oneof_f2() -> void:
		data[18].state = PB_SERVICE_STATE.UNFILLED
		_f_oneof_f2.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
	func set_f_oneof_f2(value : int) -> void:
		_f_oneof_f1.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
		data[17].state = PB_SERVICE_STATE.UNFILLED
		data[18].state = PB_SERVICE_STATE.FILLED
		_f_oneof_f2.value = value
	
	var _f_empty_out: PBField
	func get_f_empty_out() -> Test0:
		return _f_empty_out.value
	func clear_f_empty_out() -> void:
		data[19].state = PB_SERVICE_STATE.UNFILLED
		_f_empty_out.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_f_empty_out() -> Test0:
		_f_empty_out.value = Test0.new()
		return _f_empty_out.value
	
	var _f_enum_out: PBField
	func get_f_enum_out():
		return _f_enum_out.value
	func clear_f_enum_out() -> void:
		data[20].state = PB_SERVICE_STATE.UNFILLED
		_f_enum_out.value = DEFAULT_VALUES_3[PB_DATA_TYPE.ENUM]
	func set_f_enum_out(value) -> void:
		_f_enum_out.value = value
	
	var _f_empty_inner: PBField
	func get_f_empty_inner() -> Test2.TestInner2:
		return _f_empty_inner.value
	func clear_f_empty_inner() -> void:
		data[21].state = PB_SERVICE_STATE.UNFILLED
		_f_empty_inner.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_f_empty_inner() -> Test2.TestInner2:
		_f_empty_inner.value = Test2.TestInner2.new()
		return _f_empty_inner.value
	
	var _f_enum_inner: PBField
	func get_f_enum_inner():
		return _f_enum_inner.value
	func clear_f_enum_inner() -> void:
		data[22].state = PB_SERVICE_STATE.UNFILLED
		_f_enum_inner.value = DEFAULT_VALUES_3[PB_DATA_TYPE.ENUM]
	func set_f_enum_inner(value) -> void:
		_f_enum_inner.value = value
	
	var _rf_double: PBField
	func get_rf_double() -> Array:
		return _rf_double.value
	func clear_rf_double() -> void:
		data[23].state = PB_SERVICE_STATE.UNFILLED
		_rf_double.value = []
	func add_rf_double(value : float) -> void:
		_rf_double.value.append(value)
	
	var _rf_float: PBField
	func get_rf_float() -> Array:
		return _rf_float.value
	func clear_rf_float() -> void:
		data[24].state = PB_SERVICE_STATE.UNFILLED
		_rf_float.value = []
	func add_rf_float(value : float) -> void:
		_rf_float.value.append(value)
	
	var _rf_int32: PBField
	func get_rf_int32() -> Array:
		return _rf_int32.value
	func clear_rf_int32() -> void:
		data[25].state = PB_SERVICE_STATE.UNFILLED
		_rf_int32.value = []
	func add_rf_int32(value : int) -> void:
		_rf_int32.value.append(value)
	
	var _rf_int64: PBField
	func get_rf_int64() -> Array:
		return _rf_int64.value
	func clear_rf_int64() -> void:
		data[26].state = PB_SERVICE_STATE.UNFILLED
		_rf_int64.value = []
	func add_rf_int64(value : int) -> void:
		_rf_int64.value.append(value)
	
	var _rf_uint32: PBField
	func get_rf_uint32() -> Array:
		return _rf_uint32.value
	func clear_rf_uint32() -> void:
		data[27].state = PB_SERVICE_STATE.UNFILLED
		_rf_uint32.value = []
	func add_rf_uint32(value : int) -> void:
		_rf_uint32.value.append(value)
	
	var _rf_uint64: PBField
	func get_rf_uint64() -> Array:
		return _rf_uint64.value
	func clear_rf_uint64() -> void:
		data[28].state = PB_SERVICE_STATE.UNFILLED
		_rf_uint64.value = []
	func add_rf_uint64(value : int) -> void:
		_rf_uint64.value.append(value)
	
	var _rf_sint32: PBField
	func get_rf_sint32() -> Array:
		return _rf_sint32.value
	func clear_rf_sint32() -> void:
		data[29].state = PB_SERVICE_STATE.UNFILLED
		_rf_sint32.value = []
	func add_rf_sint32(value : int) -> void:
		_rf_sint32.value.append(value)
	
	var _rf_sint64: PBField
	func get_rf_sint64() -> Array:
		return _rf_sint64.value
	func clear_rf_sint64() -> void:
		data[30].state = PB_SERVICE_STATE.UNFILLED
		_rf_sint64.value = []
	func add_rf_sint64(value : int) -> void:
		_rf_sint64.value.append(value)
	
	var _rf_fixed32: PBField
	func get_rf_fixed32() -> Array:
		return _rf_fixed32.value
	func clear_rf_fixed32() -> void:
		data[31].state = PB_SERVICE_STATE.UNFILLED
		_rf_fixed32.value = []
	func add_rf_fixed32(value : int) -> void:
		_rf_fixed32.value.append(value)
	
	var _rf_fixed64: PBField
	func get_rf_fixed64() -> Array:
		return _rf_fixed64.value
	func clear_rf_fixed64() -> void:
		data[32].state = PB_SERVICE_STATE.UNFILLED
		_rf_fixed64.value = []
	func add_rf_fixed64(value : int) -> void:
		_rf_fixed64.value.append(value)
	
	var _rf_sfixed32: PBField
	func get_rf_sfixed32() -> Array:
		return _rf_sfixed32.value
	func clear_rf_sfixed32() -> void:
		data[33].state = PB_SERVICE_STATE.UNFILLED
		_rf_sfixed32.value = []
	func add_rf_sfixed32(value : int) -> void:
		_rf_sfixed32.value.append(value)
	
	var _rf_sfixed64: PBField
	func get_rf_sfixed64() -> Array:
		return _rf_sfixed64.value
	func clear_rf_sfixed64() -> void:
		data[34].state = PB_SERVICE_STATE.UNFILLED
		_rf_sfixed64.value = []
	func add_rf_sfixed64(value : int) -> void:
		_rf_sfixed64.value.append(value)
	
	var _rf_bool: PBField
	func get_rf_bool() -> Array:
		return _rf_bool.value
	func clear_rf_bool() -> void:
		data[35].state = PB_SERVICE_STATE.UNFILLED
		_rf_bool.value = []
	func add_rf_bool(value : bool) -> void:
		_rf_bool.value.append(value)
	
	var _rf_string: PBField
	func get_rf_string() -> Array:
		return _rf_string.value
	func clear_rf_string() -> void:
		data[36].state = PB_SERVICE_STATE.UNFILLED
		_rf_string.value = []
	func add_rf_string(value : String) -> void:
		_rf_string.value.append(value)
	
	var _rf_bytes: PBField
	func get_rf_bytes() -> Array:
		return _rf_bytes.value
	func clear_rf_bytes() -> void:
		data[37].state = PB_SERVICE_STATE.UNFILLED
		_rf_bytes.value = []
	func add_rf_bytes(value : PoolByteArray) -> void:
		_rf_bytes.value.append(value)
	
	var _rf_empty_out: PBField
	func get_rf_empty_out() -> Array:
		return _rf_empty_out.value
	func clear_rf_empty_out() -> void:
		data[38].state = PB_SERVICE_STATE.UNFILLED
		_rf_empty_out.value = []
	func add_rf_empty_out() -> Test0:
		var element = Test0.new()
		_rf_empty_out.value.append(element)
		return element
	
	var _rf_enum_out: PBField
	func get_rf_enum_out() -> Array:
		return _rf_enum_out.value
	func clear_rf_enum_out() -> void:
		data[39].state = PB_SERVICE_STATE.UNFILLED
		_rf_enum_out.value = []
	func add_rf_enum_out(value) -> void:
		_rf_enum_out.value.append(value)
	
	var _rf_empty_inner: PBField
	func get_rf_empty_inner() -> Array:
		return _rf_empty_inner.value
	func clear_rf_empty_inner() -> void:
		data[40].state = PB_SERVICE_STATE.UNFILLED
		_rf_empty_inner.value = []
	func add_rf_empty_inner() -> Test2.TestInner2:
		var element = Test2.TestInner2.new()
		_rf_empty_inner.value.append(element)
		return element
	
	var _rf_enum_inner: PBField
	func get_rf_enum_inner() -> Array:
		return _rf_enum_inner.value
	func clear_rf_enum_inner() -> void:
		data[41].state = PB_SERVICE_STATE.UNFILLED
		_rf_enum_inner.value = []
	func add_rf_enum_inner(value) -> void:
		_rf_enum_inner.value.append(value)
	
	var _rfu_double: PBField
	func get_rfu_double() -> Array:
		return _rfu_double.value
	func clear_rfu_double() -> void:
		data[42].state = PB_SERVICE_STATE.UNFILLED
		_rfu_double.value = []
	func add_rfu_double(value : float) -> void:
		_rfu_double.value.append(value)
	
	var _rfu_float: PBField
	func get_rfu_float() -> Array:
		return _rfu_float.value
	func clear_rfu_float() -> void:
		data[43].state = PB_SERVICE_STATE.UNFILLED
		_rfu_float.value = []
	func add_rfu_float(value : float) -> void:
		_rfu_float.value.append(value)
	
	var _rfu_int32f: PBField
	func get_rfu_int32f() -> Array:
		return _rfu_int32f.value
	func clear_rfu_int32f() -> void:
		data[44].state = PB_SERVICE_STATE.UNFILLED
		_rfu_int32f.value = []
	func add_rfu_int32f(value : int) -> void:
		_rfu_int32f.value.append(value)
	
	var _rfu_int64f: PBField
	func get_rfu_int64f() -> Array:
		return _rfu_int64f.value
	func clear_rfu_int64f() -> void:
		data[45].state = PB_SERVICE_STATE.UNFILLED
		_rfu_int64f.value = []
	func add_rfu_int64f(value : int) -> void:
		_rfu_int64f.value.append(value)
	
	var _rfu_uint32: PBField
	func get_rfu_uint32() -> Array:
		return _rfu_uint32.value
	func clear_rfu_uint32() -> void:
		data[46].state = PB_SERVICE_STATE.UNFILLED
		_rfu_uint32.value = []
	func add_rfu_uint32(value : int) -> void:
		_rfu_uint32.value.append(value)
	
	var _rfu_uint64: PBField
	func get_rfu_uint64() -> Array:
		return _rfu_uint64.value
	func clear_rfu_uint64() -> void:
		data[47].state = PB_SERVICE_STATE.UNFILLED
		_rfu_uint64.value = []
	func add_rfu_uint64(value : int) -> void:
		_rfu_uint64.value.append(value)
	
	var _rfu_sint32: PBField
	func get_rfu_sint32() -> Array:
		return _rfu_sint32.value
	func clear_rfu_sint32() -> void:
		data[48].state = PB_SERVICE_STATE.UNFILLED
		_rfu_sint32.value = []
	func add_rfu_sint32(value : int) -> void:
		_rfu_sint32.value.append(value)
	
	var _rfu_sint64: PBField
	func get_rfu_sint64() -> Array:
		return _rfu_sint64.value
	func clear_rfu_sint64() -> void:
		data[49].state = PB_SERVICE_STATE.UNFILLED
		_rfu_sint64.value = []
	func add_rfu_sint64(value : int) -> void:
		_rfu_sint64.value.append(value)
	
	var _rfu_fixed32: PBField
	func get_rfu_fixed32() -> Array:
		return _rfu_fixed32.value
	func clear_rfu_fixed32() -> void:
		data[50].state = PB_SERVICE_STATE.UNFILLED
		_rfu_fixed32.value = []
	func add_rfu_fixed32(value : int) -> void:
		_rfu_fixed32.value.append(value)
	
	var _rfu_fixed64: PBField
	func get_rfu_fixed64() -> Array:
		return _rfu_fixed64.value
	func clear_rfu_fixed64() -> void:
		data[51].state = PB_SERVICE_STATE.UNFILLED
		_rfu_fixed64.value = []
	func add_rfu_fixed64(value : int) -> void:
		_rfu_fixed64.value.append(value)
	
	var _rfu_sfixed32: PBField
	func get_rfu_sfixed32() -> Array:
		return _rfu_sfixed32.value
	func clear_rfu_sfixed32() -> void:
		data[52].state = PB_SERVICE_STATE.UNFILLED
		_rfu_sfixed32.value = []
	func add_rfu_sfixed32(value : int) -> void:
		_rfu_sfixed32.value.append(value)
	
	var _rfu_sfixed64: PBField
	func get_rfu_sfixed64() -> Array:
		return _rfu_sfixed64.value
	func clear_rfu_sfixed64() -> void:
		data[53].state = PB_SERVICE_STATE.UNFILLED
		_rfu_sfixed64.value = []
	func add_rfu_sfixed64(value : int) -> void:
		_rfu_sfixed64.value.append(value)
	
	var _rfu_bool: PBField
	func get_rfu_bool() -> Array:
		return _rfu_bool.value
	func clear_rfu_bool() -> void:
		data[54].state = PB_SERVICE_STATE.UNFILLED
		_rfu_bool.value = []
	func add_rfu_bool(value : bool) -> void:
		_rfu_bool.value.append(value)
	
	var _rf_inner: PBField
	func get_rf_inner() -> Array:
		return _rf_inner.value
	func clear_rf_inner() -> void:
		data[55].state = PB_SERVICE_STATE.UNFILLED
		_rf_inner.value = []
	func add_rf_inner() -> Test2.TestInner3.TestInner3_2:
		var element = Test2.TestInner3.TestInner3_2.new()
		_rf_inner.value.append(element)
		return element
	
	class map_type_f_map:
		func _init():
			var service
			
			_key = PBField.new("key", PB_DATA_TYPE.INT32, PB_RULE.REQUIRED, 1, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
			_key.is_map_field = true
			service = PBServiceField.new()
			service.field = _key
			data[_key.tag] = service
			
			_value = PBField.new("value", PB_DATA_TYPE.INT32, PB_RULE.REQUIRED, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
			_value.is_map_field = true
			service = PBServiceField.new()
			service.field = _value
			data[_value.tag] = service
			
		var data = {}
		
		var _key: PBField
		func get_key() -> int:
			return _key.value
		func clear_key() -> void:
			data[1].state = PB_SERVICE_STATE.UNFILLED
			_key.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
		func set_key(value : int) -> void:
			_key.value = value
		
		var _value: PBField
		func get_value() -> int:
			return _value.value
		func clear_value() -> void:
			data[2].state = PB_SERVICE_STATE.UNFILLED
			_value.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
		func set_value(value : int) -> void:
			_value.value = value
		
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
	
class Test2:
	func _init():
		var service
		
		_f1 = PBField.new("f1", PB_DATA_TYPE.STRING, PB_RULE.REPEATED, 1, true, [])
		service = PBServiceField.new()
		service.field = _f1
		data[_f1.tag] = service
		
		_f2 = PBField.new("f2", PB_DATA_TYPE.FIXED64, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FIXED64])
		service = PBServiceField.new()
		service.field = _f2
		data[_f2.tag] = service
		
		_f3 = PBField.new("f3", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 3, true, DEFAULT_VALUES_3[PB_DATA_TYPE.STRING])
		service = PBServiceField.new()
		service.field = _f3
		data[_f3.tag] = service
		
		_f4 = PBField.new("f4", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 4, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _f4
		service.func_ref = funcref(self, "new_f4")
		data[_f4.tag] = service
		
		_f5 = PBField.new("f5", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 5, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _f5
		service.func_ref = funcref(self, "new_f5")
		data[_f5.tag] = service
		
		_f6 = PBField.new("f6", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 6, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _f6
		service.func_ref = funcref(self, "new_f6")
		data[_f6.tag] = service
		
		_f7 = PBField.new("f7", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 7, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
		service = PBServiceField.new()
		service.field = _f7
		service.func_ref = funcref(self, "new_f7")
		data[_f7.tag] = service
		
	var data = {}
	
	var _f1: PBField
	func get_f1() -> Array:
		return _f1.value
	func clear_f1() -> void:
		data[1].state = PB_SERVICE_STATE.UNFILLED
		_f1.value = []
	func add_f1(value : String) -> void:
		_f1.value.append(value)
	
	var _f2: PBField
	func get_f2() -> int:
		return _f2.value
	func clear_f2() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_f2.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FIXED64]
	func set_f2(value : int) -> void:
		_f2.value = value
	
	var _f3: PBField
	func has_f3() -> bool:
		return data[3].state == PB_SERVICE_STATE.FILLED
	func get_f3() -> String:
		return _f3.value
	func clear_f3() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_f3.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
	func set_f3(value : String) -> void:
		data[3].state = PB_SERVICE_STATE.FILLED
		_f4.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
		data[4].state = PB_SERVICE_STATE.UNFILLED
		_f3.value = value
	
	var _f4: PBField
	func has_f4() -> bool:
		return data[4].state == PB_SERVICE_STATE.FILLED
	func get_f4() -> Test2.TestInner3:
		return _f4.value
	func clear_f4() -> void:
		data[4].state = PB_SERVICE_STATE.UNFILLED
		_f4.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_f4() -> Test2.TestInner3:
		_f3.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
		data[3].state = PB_SERVICE_STATE.UNFILLED
		data[4].state = PB_SERVICE_STATE.FILLED
		_f4.value = Test2.TestInner3.new()
		return _f4.value
	
	var _f5: PBField
	func get_f5() -> Test2.TestInner2:
		return _f5.value
	func clear_f5() -> void:
		data[5].state = PB_SERVICE_STATE.UNFILLED
		_f5.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_f5() -> Test2.TestInner2:
		_f5.value = Test2.TestInner2.new()
		return _f5.value
	
	var _f6: PBField
	func get_f6() -> Test2.TestInner3:
		return _f6.value
	func clear_f6() -> void:
		data[6].state = PB_SERVICE_STATE.UNFILLED
		_f6.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_f6() -> Test2.TestInner3:
		_f6.value = Test2.TestInner3.new()
		return _f6.value
	
	var _f7: PBField
	func get_f7() -> Test2.TestInner1:
		return _f7.value
	func clear_f7() -> void:
		data[7].state = PB_SERVICE_STATE.UNFILLED
		_f7.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
	func new_f7() -> Test2.TestInner1:
		_f7.value = Test2.TestInner1.new()
		return _f7.value
	
	enum TestEnum {
		VALUE_0 = 0,
		VALUE_1 = 1,
		VALUE_2 = 2,
		VALUE_3 = 3
	}
	
	class TestInner1:
		func _init():
			var service
			
			_f1 = PBField.new("f1", PB_DATA_TYPE.DOUBLE, PB_RULE.REPEATED, 1, true, [])
			service = PBServiceField.new()
			service.field = _f1
			data[_f1.tag] = service
			
			_f2 = PBField.new("f2", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
			service = PBServiceField.new()
			service.field = _f2
			data[_f2.tag] = service
			
			_f3 = PBField.new("f3", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 3, true, DEFAULT_VALUES_3[PB_DATA_TYPE.STRING])
			service = PBServiceField.new()
			service.field = _f3
			data[_f3.tag] = service
			
		var data = {}
		
		var _f1: PBField
		func get_f1() -> Array:
			return _f1.value
		func clear_f1() -> void:
			data[1].state = PB_SERVICE_STATE.UNFILLED
			_f1.value = []
		func add_f1(value : float) -> void:
			_f1.value.append(value)
		
		var _f2: PBField
		func get_f2() -> float:
			return _f2.value
		func clear_f2() -> void:
			data[2].state = PB_SERVICE_STATE.UNFILLED
			_f2.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
		func set_f2(value : float) -> void:
			_f2.value = value
		
		var _f3: PBField
		func get_f3() -> String:
			return _f3.value
		func clear_f3() -> void:
			data[3].state = PB_SERVICE_STATE.UNFILLED
			_f3.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
		func set_f3(value : String) -> void:
			_f3.value = value
		
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
		
	class TestInner2:
		func _init():
			var service
			
		var data = {}
		
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
		
	class TestInner3:
		func _init():
			var service
			
			_f1 = PBField.new("f1", PB_DATA_TYPE.MAP, PB_RULE.REPEATED, 1, true, [])
			service = PBServiceField.new()
			service.field = _f1
			service.func_ref = funcref(self, "add_empty_f1")
			data[_f1.tag] = service
			
			_f2 = PBField.new("f2", PB_DATA_TYPE.ENUM, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.ENUM])
			service = PBServiceField.new()
			service.field = _f2
			data[_f2.tag] = service
			
			_f3 = PBField.new("f3", PB_DATA_TYPE.MESSAGE, PB_RULE.OPTIONAL, 3, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
			service = PBServiceField.new()
			service.field = _f3
			service.func_ref = funcref(self, "new_f3")
			data[_f3.tag] = service
			
		var data = {}
		
		var _f1: PBField
		func get_raw_f1():
			return _f1.value
		func get_f1():
			return PBPacker.construct_map(_f1.value)
		func clear_f1():
			data[1].state = PB_SERVICE_STATE.UNFILLED
			_f1.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MAP]
		func add_empty_f1() -> Test2.TestInner3.map_type_f1:
			var element = Test2.TestInner3.map_type_f1.new()
			_f1.value.append(element)
			return element
		func add_f1(a_key) -> Test2.TestInner3.map_type_f1:
			var idx = -1
			for i in range(_f1.value.size()):
				if _f1.value[i].get_key() == a_key:
					idx = i
					break
			var element = Test2.TestInner3.map_type_f1.new()
			element.set_key(a_key)
			if idx != -1:
				_f1.value[idx] = element
			else:
				_f1.value.append(element)
			return element.new_value()
		
		var _f2: PBField
		func get_f2():
			return _f2.value
		func clear_f2() -> void:
			data[2].state = PB_SERVICE_STATE.UNFILLED
			_f2.value = DEFAULT_VALUES_3[PB_DATA_TYPE.ENUM]
		func set_f2(value) -> void:
			_f2.value = value
		
		var _f3: PBField
		func get_f3() -> Test2.TestInner3.TestInner3_1:
			return _f3.value
		func clear_f3() -> void:
			data[3].state = PB_SERVICE_STATE.UNFILLED
			_f3.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
		func new_f3() -> Test2.TestInner3.TestInner3_1:
			_f3.value = Test2.TestInner3.TestInner3_1.new()
			return _f3.value
		
		class TestInner3_1:
			func _init():
				var service
				
			var data = {}
			
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
			
		class TestInner3_2:
			func _init():
				var service
				
				_f1 = PBField.new("f1", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 1, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
				service = PBServiceField.new()
				service.field = _f1
				data[_f1.tag] = service
				
				_f2 = PBField.new("f2", PB_DATA_TYPE.UINT64, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.UINT64])
				service = PBServiceField.new()
				service.field = _f2
				data[_f2.tag] = service
				
			var data = {}
			
			var _f1: PBField
			func get_f1() -> int:
				return _f1.value
			func clear_f1() -> void:
				data[1].state = PB_SERVICE_STATE.UNFILLED
				_f1.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
			func set_f1(value : int) -> void:
				_f1.value = value
			
			var _f2: PBField
			func get_f2() -> int:
				return _f2.value
			func clear_f2() -> void:
				data[2].state = PB_SERVICE_STATE.UNFILLED
				_f2.value = DEFAULT_VALUES_3[PB_DATA_TYPE.UINT64]
			func set_f2(value : int) -> void:
				_f2.value = value
			
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
			
		class map_type_f1:
			func _init():
				var service
				
				_key = PBField.new("key", PB_DATA_TYPE.STRING, PB_RULE.REQUIRED, 1, true, DEFAULT_VALUES_3[PB_DATA_TYPE.STRING])
				_key.is_map_field = true
				service = PBServiceField.new()
				service.field = _key
				data[_key.tag] = service
				
				_value = PBField.new("value", PB_DATA_TYPE.MESSAGE, PB_RULE.REQUIRED, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE])
				_value.is_map_field = true
				service = PBServiceField.new()
				service.field = _value
				service.func_ref = funcref(self, "new_value")
				data[_value.tag] = service
				
			var data = {}
			
			var _key: PBField
			func get_key() -> String:
				return _key.value
			func clear_key() -> void:
				data[1].state = PB_SERVICE_STATE.UNFILLED
				_key.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
			func set_key(value : String) -> void:
				_key.value = value
			
			var _value: PBField
			func get_value() -> Test2.TestInner3.TestInner3_2:
				return _value.value
			func clear_value() -> void:
				data[2].state = PB_SERVICE_STATE.UNFILLED
				_value.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MESSAGE]
			func new_value() -> Test2.TestInner3.TestInner3_2:
				_value.value = Test2.TestInner3.TestInner3_2.new()
				return _value.value
			
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
	
class Test4:
	func _init():
		var service
		
		_f1 = PBField.new("f1", PB_DATA_TYPE.INT32, PB_RULE.OPTIONAL, 10, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
		service = PBServiceField.new()
		service.field = _f1
		data[_f1.tag] = service
		
		_f2 = PBField.new("f2", PB_DATA_TYPE.STRING, PB_RULE.OPTIONAL, 3, true, DEFAULT_VALUES_3[PB_DATA_TYPE.STRING])
		service = PBServiceField.new()
		service.field = _f2
		data[_f2.tag] = service
		
		_f3 = PBField.new("f3", PB_DATA_TYPE.FLOAT, PB_RULE.OPTIONAL, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT])
		service = PBServiceField.new()
		service.field = _f3
		data[_f3.tag] = service
		
		_f4 = PBField.new("f4", PB_DATA_TYPE.DOUBLE, PB_RULE.OPTIONAL, 160, true, DEFAULT_VALUES_3[PB_DATA_TYPE.DOUBLE])
		service = PBServiceField.new()
		service.field = _f4
		data[_f4.tag] = service
		
		_f5 = PBField.new("f5", PB_DATA_TYPE.MAP, PB_RULE.REPEATED, 99, true, [])
		service = PBServiceField.new()
		service.field = _f5
		service.func_ref = funcref(self, "add_empty_f5")
		data[_f5.tag] = service
		
	var data = {}
	
	var _f1: PBField
	func get_f1() -> int:
		return _f1.value
	func clear_f1() -> void:
		data[10].state = PB_SERVICE_STATE.UNFILLED
		_f1.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
	func set_f1(value : int) -> void:
		_f1.value = value
	
	var _f2: PBField
	func get_f2() -> String:
		return _f2.value
	func clear_f2() -> void:
		data[3].state = PB_SERVICE_STATE.UNFILLED
		_f2.value = DEFAULT_VALUES_3[PB_DATA_TYPE.STRING]
	func set_f2(value : String) -> void:
		_f2.value = value
	
	var _f3: PBField
	func get_f3() -> float:
		return _f3.value
	func clear_f3() -> void:
		data[2].state = PB_SERVICE_STATE.UNFILLED
		_f3.value = DEFAULT_VALUES_3[PB_DATA_TYPE.FLOAT]
	func set_f3(value : float) -> void:
		_f3.value = value
	
	var _f4: PBField
	func get_f4() -> float:
		return _f4.value
	func clear_f4() -> void:
		data[160].state = PB_SERVICE_STATE.UNFILLED
		_f4.value = DEFAULT_VALUES_3[PB_DATA_TYPE.DOUBLE]
	func set_f4(value : float) -> void:
		_f4.value = value
	
	var _f5: PBField
	func get_raw_f5():
		return _f5.value
	func get_f5():
		return PBPacker.construct_map(_f5.value)
	func clear_f5():
		data[99].state = PB_SERVICE_STATE.UNFILLED
		_f5.value = DEFAULT_VALUES_3[PB_DATA_TYPE.MAP]
	func add_empty_f5() -> int:
		var element = Test4.map_type_f5.new()
		_f5.value.append(element)
		return element
	func add_f5(a_key, a_value) -> void:
		var idx = -1
		for i in range(_f5.value.size()):
			if _f5.value[i].get_key() == a_key:
				idx = i
				break
		var element = Test4.map_type_f5.new()
		element.set_key(a_key)
		element.set_value(a_value)
		if idx != -1:
			_f5.value[idx] = element
		else:
			_f5.value.append(element)
	
	class map_type_f5:
		func _init():
			var service
			
			_key = PBField.new("key", PB_DATA_TYPE.INT32, PB_RULE.REQUIRED, 1, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
			_key.is_map_field = true
			service = PBServiceField.new()
			service.field = _key
			data[_key.tag] = service
			
			_value = PBField.new("value", PB_DATA_TYPE.INT32, PB_RULE.REQUIRED, 2, true, DEFAULT_VALUES_3[PB_DATA_TYPE.INT32])
			_value.is_map_field = true
			service = PBServiceField.new()
			service.field = _value
			data[_value.tag] = service
			
		var data = {}
		
		var _key: PBField
		func get_key() -> int:
			return _key.value
		func clear_key() -> void:
			data[1].state = PB_SERVICE_STATE.UNFILLED
			_key.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
		func set_key(value : int) -> void:
			_key.value = value
		
		var _value: PBField
		func get_value() -> int:
			return _value.value
		func clear_value() -> void:
			data[2].state = PB_SERVICE_STATE.UNFILLED
			_value.value = DEFAULT_VALUES_3[PB_DATA_TYPE.INT32]
		func set_value(value : int) -> void:
			_value.value = value
		
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
