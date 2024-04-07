import ctypes

class CStructBase:
    fields = []

    def create_ctypes_struct(self):
        """
        Dynamically create a ctypes.Structure class based on the fields defined in the derived class.
        """
        class_fields = []
        for field_name, field_type in self.fields:
            if hasattr(field_type, '_length_'):  # It's an array
                base_type = field_type._type_
                c_type = base_type * field_type._length_
            else:
                c_type = field_type
            class_fields.append((field_name, c_type))

        # Dynamically create the ctypes structure class
        return type(f'{self.__class__.__name__}CStruct', (ctypes.Structure,), {'_fields_': class_fields})

    def serialize(self):
        """
        Serialize an instance of the derived class into a byte array matching the C struct layout.
        """
        CStruct = self.create_ctypes_struct()
        c_struct_instance = CStruct()

        for field_name, field_type in self.fields:
            value = getattr(self, field_name)
            setattr(c_struct_instance, field_name, value)

        return bytearray(c_struct_instance)

    @classmethod
    def generate_c_struct_code(cls):
        """
        Generate the C struct code based on the derived class fields.
        """
        lines = ["typedef struct __attribute__((packed)) {"]
        for field_name, field_type in cls.fields:
            if hasattr(field_type, '_length_'):
                base_type = field_type._type_.__name__.replace('c_', '')
                lines.append(f"  {base_type} {field_name}[{field_type._length_}];")
            else:
                base_type = field_type.__name__.replace('c_', '')
                lines.append(f"  {base_type} {field_name};")
        lines.append(f"}} {cls.__name__}_t;")
        return "\n".join(lines)