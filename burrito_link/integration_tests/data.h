#ifndef BURRITO_LINK_INTEGRATION_TEST_DATA_H_
#define BURRITO_LINK_INTEGRATION_TEST_DATA_H_

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VARTYPE_BYTE (size_t)0
#define VARTYPE_FLOAT (size_t)1
#define VARTYPE_UNSIGNED_INT32 (size_t)2
#define VARTYPE_STRING (size_t)3
#define VARTYPE_UNSIGNED_INT16 (size_t)4

const size_t variable_lengths[] = {
    sizeof(uint8_t),
    sizeof(float),
    sizeof(uint32_t),
    0,
    sizeof(uint16_t),
};

struct SizedBuffer {
    uint8_t* buffer;
    size_t size;
};

struct SizedBuffer my_sum_and_call(size_t num_args, ...) {
    size_t length = 0;
    va_list args;
    va_start(args, num_args);
    for (size_t i = 0; i < num_args; i += 2) {
        size_t vartype = va_arg(args, size_t);
        if (vartype == VARTYPE_STRING) {
            char* data = va_arg(args, char*);
            length += strlen(data);
            // We need to include the null byte because WideCharToMultiByte includes it when cchWideChar is -1
            length += 1;
        }
        else {
            length += variable_lengths[vartype];
            va_arg(args, void*);
        }
    }
    va_end(args);
    // printf("Hello world starting processing %zu\n", length);
    uint8_t* buffer = malloc(length * sizeof(uint8_t));
    size_t buffer_position = 0;

    va_start(args, num_args);
    for (size_t i = 0; i < num_args; i += 2) {
        uint32_t vartype = va_arg(args, size_t);
        if (vartype == VARTYPE_BYTE) {
            int data = va_arg(args, int);
            memcpy(buffer + buffer_position, &data, 1);
            buffer_position += 1;
        }
        else if (vartype == VARTYPE_FLOAT) {
            float data = va_arg(args, double);
            memcpy(buffer + buffer_position, &data, 4);
            buffer_position += 4;
        }
        else if (vartype == VARTYPE_UNSIGNED_INT32) {
            uint32_t data = va_arg(args, uint32_t);
            memcpy(buffer + buffer_position, &data, 4);
            buffer_position += 4;
        }
        else if (vartype == VARTYPE_STRING) {
            char* data = va_arg(args, char*);
            size_t length = strlen(data);

            // We need to include the null byte because WideCharToMultiByte includes it when cchWideChar is -1
            length += 1;
            memcpy(buffer + buffer_position, data, length);
            buffer_position += length;
        }
        else if (vartype == VARTYPE_UNSIGNED_INT16) {
            uint16_t data = va_arg(args, uint32_t);
            memcpy(buffer + buffer_position, &data, 2);
            buffer_position += 2;
        }
        else {
            printf("ERROR, unknown type");
        }
    }
    va_end(args);
    // printf("Hello World, done processing. %zu %zu\n", length, num_args);

    // for (size_t i = 0; i < length; i++) {
    //     printf("%02X", buffer[i]);
    // }
    struct SizedBuffer sized_buffer;
    sized_buffer.buffer = buffer;
    sized_buffer.size = length;
    return sized_buffer;
}

void mask_bytes(size_t start_byte, uint32_t count, uint8_t* buffer) {
    for (size_t i = 0; i < count; i++) {
        buffer[i + start_byte] = 0xFF;
    }
}

#define MAKETESTCASE_BYTES(...) my_sum_and_call(__VA_ARGS__)
#define B(a) VARTYPE_BYTE, (uint8_t)a
#define F(a) VARTYPE_FLOAT, (float)a
#define U(a) VARTYPE_UNSIGNED_INT32, (uint32_t)a
#define S(a) VARTYPE_STRING, (char*)a
#define U16(a) VARTYPE_UNSIGNED_INT16, (uint16_t)a

struct SizedBuffer* test_cases_1;
struct SizedBuffer test_case_heavy;

const size_t TESTCASE_COUNT = 1020;
void build_testcases() {
    // Build Testcases for the LightData
    test_cases_1 = malloc(TESTCASE_COUNT * sizeof(struct SizedBuffer));
    test_cases_1[0] = MAKETESTCASE_BYTES(30, B(1), F(0.0), F(2.0), F(3.0), F(1.11), F(2.22), F(3.33), F(6.4), F(6.4), F(6.4), F(-123.0), F(-124.0), F(2.5), F(130.0), U(0));
    test_cases_1[1] = MAKETESTCASE_BYTES(30, B(1), F(1.0), F(2.0), F(3.0), F(1.11), F(2.22), F(3.33), F(12.8), F(12.8), F(12.8), F(-123.0), F(-124.0), F(2.5), F(130.0), U(0));
    test_cases_1[2] = MAKETESTCASE_BYTES(30, B(1), F(2.0), F(2.0), F(3.0), F(1.11), F(2.22), F(3.33), F(19.2), F(19.2), F(19.2), F(-123.0), F(-124.0), F(2.5), F(130.0), U(0));
    test_cases_1[3] = MAKETESTCASE_BYTES(30, B(1), F(3.0), F(2.0), F(3.0), F(1.11), F(2.22), F(3.33), F(25.6), F(25.6), F(25.6), F(-123.0), F(-124.0), F(2.5), F(130.0), U(0));

    for (size_t i = 4; i < TESTCASE_COUNT; i++) {
        float floati = i;
        test_cases_1[i] = MAKETESTCASE_BYTES(30, B(1), F(floati), F(2.0), F(3.0), F(1.11), F(2.22), F(3.33), F(32.0), F(32.0), F(32.0), F(-123.0), F(-124.0), F(2.5), F(130.0), U(0));
    }

    // Build Testcases for the HeavyData
    test_case_heavy = MAKETESTCASE_BYTES(
        14, B(2),
        U16(12),  // Compass Width
        U16(300),  // Compass Height
        U(12345),  // Map ID
        U(4294967295),  // x11 window ID
        U(183),  // String Buffer Length
        S("{\n  \"name\": \"Irwene\",\n  \"profession\": 4,\n  \"spec\": 55,\n  \"race\": 4,\n  \"map_id\": 50,\n  \"world_id\": 268435505,\n  \"team_color_id\": 0,\n  \"commander\": false,\n  \"fov\": 0.873,\n  \"uisz\": 1\n}")  // Identify JSON
    );
}

struct SizedBuffer get_testcase(size_t n) {
    if (n >= TESTCASE_COUNT) {
        n = TESTCASE_COUNT - 1;
    }
    return test_cases_1[n];
}

int compare(uint8_t* buffer_1, size_t buffer_len_1, uint8_t* buffer_2, size_t buffer_len_2) {
    if (buffer_len_1 != buffer_len_2) {
        printf("Lengths Differ %lli %lli\n", buffer_len_1, buffer_len_2);
        for (size_t j = 0; j < buffer_len_1; j++) {
            printf("%02X", buffer_1[j]);
        }
        printf("\n");
        for (size_t j = 0; j < buffer_len_2; j++) {
            printf("%02X", buffer_2[j]);
        }
        printf("\n");
        return 0;
    }

    for (size_t i = 0; i < buffer_len_1; i++) {
        if (buffer_1[i] != buffer_2[i]) {
            for (size_t j = 0; j < buffer_len_1; j++) {
                printf("%02X", buffer_1[j]);
            }
            printf("\n");
            for (size_t j = 0; j < buffer_len_2; j++) {
                printf("%02X", buffer_2[j]);
            }
            printf("\n");

            return 0;
        }
    }
    return 1;
}

#endif // BURRITO_LINK_INTEGRATION_TEST_DATA_H_
