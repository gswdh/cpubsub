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
    globals().update({k: getattr(module, k) for k in dir(module) if not k.startswith('_')})

    # Make the output file
    f = open(output, "w+")

    # Header file requirements
    f.write("#ifndef __MESSAGES_H__\n")
    f.write("#define __MESSAGES_H__\n")
    f.write("\n")
    f.write("#include <stdint.h>\n")
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
    f.close()

if __name__ == "__main__":
    run()



