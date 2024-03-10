import json
import click
import numpy as np

def make_struct(struct):
    struct_str = "typedef struct\n{\n\tuint32_t mid;\n"
    for field in struct["fields"]:
        struct_str += f'\t{field["type"]} {field["name"]};\n'
    struct_str += "}" + f" cPS_{struct['name']}_Packet_t;"
    return struct_str

@click.command()
@click.option("--input", help="Path to input json", required=True)
def run(input):
    with open("messages.h", "w+") as output:
        output.write("#ifndef _MESSAGES_H_\n#define _MESSAGES_H_\n\n#include <stdint.h>\n\n")
        with open(input, "r") as f:
            structs = json.load(f)
        msg_names = []
        for index, message in enumerate(structs["user_messages"]):
            msg_id = "0x" + hex(index)[2:].upper().zfill(8)
            msg_name = f"cPS_{message['name']}_MID"
            msg_names.append(msg_name)
            output.write(f"#define {msg_name} ({msg_id})\n\n")
            output.write(make_struct(message) + "\n\n")
        output.write("static const uint32_t cPS_msg_size[] = {")
        output.write(", ".join([f"sizeof({name})" for name in msg_names]))
        output.write("};\n\n")
        output.write("#endif\n")




if __name__ == "__main__":
    run()



