# cpubsub

This software is an atept to make a platform agnostic publish and subcribe the messaging manager will low respurce embedded systems in mind. 

The intention is for safe inter-thread communication. Auto generated topic IDs and message types are used to automate the communication interfaces. Only pointers are passed to the internal queues so data stored in memetoy muct be rpeserved long enough for the subscriber to make use of it. However, all memory allocated for driver use is kept within the subscriber thread and is only accessed via pointers.

## How to use
1. The pipe driver needs implementing with you local library. A FreeRTOS example is providied.
2. Add the source and include dirs to your build system.
3. Define a topic with a system wide unique ID.
4. Make a pipe in your subscriber thread with pipe_init.
5. Use the pipe object to subscribe to your topic.
6. Publish and subscribe to the topic.