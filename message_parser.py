import click
import importlib
import sys

# Add the parent directory to sys.path
sys.path.append("../")


@click.command()
@click.option("--input", help="Path to input python message classes", required=True)
@click.option("--output", help="Path to C header output file", required=True)
def run(input, output):
    # Import the messages defined in python classes
    module = importlib.import_module(input)
    globals().update(
        {k: getattr(module, k) for k in dir(module) if not k.startswith("_")}
    )

    # Generate the header file
    with open(output + ".h", "w+") as f:
        f.write("#ifndef __MESSAGES_H__\n")
        f.write("#define __MESSAGES_H__\n")
        f.write("\n")
        f.write("#include <stdint.h>\n")
        f.write("\n")

        f.write("uint32_t messages_msg_len(uint32_t mid);\n")
        f.write("\n")

        # Go through the messages and export
        for key in MSGS.keys():
            msg = MSGS[key]
            struct_name = f"{msg.__name__}"
            struct, struct_type = msg.generate_c_struct_code()
            f.write(struct + "\n")
            f.write(f"#define {struct_name}_MID ({hex(key)})\n")
            f.write(f"#define {struct_name}_LEN (sizeof({struct_type}))\n")
            f.write("\n")

        f.write("#endif\n")

    # Generate the C source file
    with open(output + ".c", "w+") as f:
        f.write('#include "messages.h"\n')
        f.write("\n")

        # Write the function to get message lengths
        f.write("uint32_t messages_msg_len(uint32_t mid) \n{\n")
        f.write("    switch (mid) {\n")

        for key in MSGS.keys():
            msg = MSGS[key]
            struct_name = f"{msg.__name__}"
            f.write(f"        case {struct_name}_MID:\n")
            f.write(f"            return {struct_name}_LEN;\n")
            f.write(f"            break;\n")

        f.write("        default:\n")
        f.write("           return 0; // Invalid MID\n")
        f.write(f"          break;\n")
        f.write("    }\n")
        f.write("}\n")


if __name__ == "__main__":
    run()
