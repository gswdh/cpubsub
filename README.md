# cpubsub

This project is an attempt to make a platform agnostic publish and subscribe with low resource embedded systems in mind. The code is written in C and only relies on standard libraries and your choice of queue library, it's targeted for compilation with a Gnu based C compiler. Your compiler should be compatible with the weak attribute for function definitions.

The intention is for safe inter-thread communication. Auto generated topic IDs and message types are used to automate the communication interfaces. Memory is only copied into queues if the queue library used implements this - memory is not managed in this library. For example, when using FreeRTOS queues, the queues copy a local version of the object making it safe to manipulate the memory after a publish. It was decided that it was desirable to be able to use one pipe with multiple message types. This means, if your queue implementation requires fixed sized objects, you will need to modify the pipe on every subscribe to accomodate the largest object that will pass through the pipe or use dedicated pipes for each message type. Even if the implementation is capable of resizing, it's likely best to seperate pipes when using messages with large difference in size. For example, if there are two message types assigned to one pipe, one is 1024 bytes in size and the other 8 bytes, 1024 bytes will always be copied even when the smaller message is published. This will waste processor time. 

## How to use
1. The pipe driver needs implementing with your local library. A FreeRTOS example is providied.
2. Add the source and include dirs to your build system.
3. Create your message types in the messages.json file, compile this with the message_parser.py script. Ideally, it's best to add this to your build system before the preprocessor.
4. Statically create a pipe object with `pipe_t pipe = {0};`
5. Use the pipe object to subscribe to your topic.
6. Create a message object and publish it with `cps_publish`
7. Receive messages with `cps_receive`.
