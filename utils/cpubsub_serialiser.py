# Define mappings for field types
TYPE_MAPPING = {
    "uint8_t": "uint8_t",
    "uint16_t": "uint16_t",
    "uint32_t": "uint32_t",
    "uint64_t": "uint64_t",
    "float": "float",
    "double": "double",
}

# Define type casting for serialization
CJSON_CAST_MAPPING = {
    "uint32_t": "(double)",
    "float": "(double)",
    "uint8_t": "(int)",  # Example for completeness
    "int": "(int)",  # Example for completeness
}


# Generate the C structs and macros
def generate_c_code(json_data, output_file_name):
    messages = json_data["messages"]
    mids = json_data["mids"]

    c_code = ["#ifndef __MSGS__\n#define __MSGS__\n\n#include <stdint.h>\n"]

    # Generate struct definitions
    for msg_name, details in messages.items():
        fields = details["fields"]

        # Prepare the struct fields
        struct_lines = []
        for field_name, field_type in fields.items():
            c_type = TYPE_MAPPING.get(field_type, field_type)
            struct_lines.append(f"  {c_type} {field_name};")

        # Generate struct definition
        struct_name = f"{msg_name}_t"
        struct_def = (
            f"typedef struct __attribute__((packed))\n{{\n"
            + "\n".join(struct_lines)
            + f"\n}} {struct_name};"
        )
        c_code.append(struct_def)

        # Add macros for MID and struct length
        mid_value = mids[msg_name]
        macros = (
            f"#define {msg_name}_MID ({mid_value})\n"
            f"#define {msg_name}_LEN (sizeof({struct_name}))\n"
        )
        c_code.append(macros)

    for msg_name, details in messages.items():
        fields = details["fields"]
        # Begin function definition
        c_code.append(
            f"char *messages_serialise_{msg_name.lower()}(const {msg_name}_t *const msg);"
        )

    c_code.append("\n#endif\n")

    with open(f"{output_file_name}.h", "w+") as f:
        f.write("\n".join(c_code))

    # Start the function
    function_lines = [
        '#include "messages.h"',
        "",
        '#include "cJSON.h"',
        "",
        "uint32_t messages_msg_len(const uint32_t mid)",
        "{",
        "    switch (mid)",
        "    {",
    ]

    # Add cases for each MID
    for msg_name in mids:
        function_lines.append(f"    case {msg_name}_MID:")
        function_lines.append(f"        return {msg_name}_LEN;")
        function_lines.append("        break;")

    # Add the default case
    function_lines.append("    default:")
    function_lines.append("        return 0; // Invalid MID")
    function_lines.append("        break;")

    # Close the switch and function
    function_lines.append("    }")
    function_lines.append("}")

    for msg_name, details in messages.items():
        fields = details["fields"]

        # Begin function definition
        _function_lines = [
            f"char *messages_serialise_{msg_name.lower()}(const {msg_name}_t *const msg)",
            "{",
            "    // User settings",
            "    cJSON *serialised = cJSON_CreateObject();",
        ]

        # Add fields to the JSON object
        for field_name, field_type in fields.items():
            cjson_cast = CJSON_CAST_MAPPING.get(field_type, "")
            _function_lines.append(
                f'    cJSON_AddItemToObject(serialised, "{field_name}", cJSON_CreateNumber({cjson_cast}msg->{field_name}));'
            )

        # Add stringification and return
        _function_lines.append("")
        _function_lines.append("    // Stringify the result")
        _function_lines.append("    return cJSON_Print(serialised);")
        _function_lines.append("}")

        # Combine function lines and add to the list
        function_lines.append("\n".join(_function_lines))

    with open(f"{output_file_name}.c", "w+") as f:
        f.write("\n".join(function_lines))
