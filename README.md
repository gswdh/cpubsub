# Pipe Subscription System (CPS)

## Overview

The **Pipe Subscription System (CPS)** is a lightweight, flexible publish-subscribe system designed for embedded systems and inter-process communication (IPC). It provides a robust mechanism for message passing between different components of a system using a pipe-based architecture.

## Features

- **Message Publishing**: Efficient message distribution to all subscribed pipes
- **Subscription Management**: Dynamic subscription to specific message types
- **Queue Management**: Configurable queue sizes and efficient message handling
- **Thread Safety**: Optional thread-safe implementations (FreeRTOS and pthread)
- **Cross-Platform**: Designed to work across different platforms
- **Memory Efficient**: Static memory allocation with configurable limits
- **Optional Features**:
  - Timestamp support
  - Network message handling
  - Queue locking for thread safety

## Configuration Options

The system can be configured through `cpubsub_config.h`:

```c
// Default configuration values
#define CPS_NETWORK_MID         (0x00000000)
#define CPS_PIPE_LENGTH_DEFAULT (10U)
#define CPS_RX_BLOCK_INT_MS     (10U)
#define CPS_MAX_SUBS            (32U)
#define CPS_MAX_MSG_SIZE        (256U)

// Optional features (uncomment to enable)
// #define CPS_HEADER_USE_TIMESTAMP
// #define CPS_HEADER_USE_TIME_H
// #define CPS_HEADER_USE_NETWORKING
// #define CPS_HEADER_USE_QUEUE_LOCK
```

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/gswdh/cpubsub.git
   ```

2. Copy the following files to your project:
   - `cpubsub.h`
   - `cpubsub.c`
   - `cpubsub_config.h`
   - `pipe.h`
   - `pipe.c`
   - `sublist.h`
   - `sublist.c`

3. Choose and implement a pipe interface:
   - For FreeRTOS: Use `examples/pipe_interface_freertos.c`
   - For pthread: Use `examples/pipe_interface_pthread.c`
   - Or implement your own interface for your platform

## Usage

### Basic Setup

1. Configure your pipe interface:
   ```c
   pipe_t my_pipe = {0};
   ```

2. Subscribe to messages:
   ```c
   cps_result_t result = cps_subscribe(&my_pipe, MSG_ID, sizeof(my_message_t));
   ```

3. Initialize message headers:
   ```c
   my_message_t msg = {0};
   cps_init_msg_header(&msg, DEVICE_ID, MSG_ID);
   ```

4. Publish messages:
   ```c
   cps_result_t result = cps_publish(&msg);
   ```

5. Receive messages:
   ```c
   my_message_t received_msg;
   cps_result_t result = cps_receive(&my_pipe, &received_msg, CPS_WAIT_BLOCK);
   ```

### Advanced Features

#### Timestamp Support
Enable timestamp support by uncommenting `CPS_HEADER_USE_TIMESTAMP` in `cpubsub_config.h`. You can either:
- Use system time (`CPS_HEADER_USE_TIME_H`)
- Implement your own time function (`cps_get_time()`)

#### Network Support
Enable network support by uncommenting `CPS_HEADER_USE_NETWORKING` in `cpubsub_config.h`. This allows:
- Network message publishing
- Device ID filtering
- Message routing

#### Thread Safety
Enable thread safety by uncommenting `CPS_HEADER_USE_QUEUE_LOCK` in `cpubsub_config.h`. This adds:
- Mutex protection for queue operations
- Thread-safe message handling

## Error Handling

The system uses the following error codes:

```c
typedef enum {
    CPS_OK = 0U,
    CPS_INVALID_PARAM,
    CPS_NO_MSG,
    CPS_SUB_ERROR,
    CPS_PUB_ERROR,
    CPS_PIPE_INIT_ERROR,
} cps_result_t;
```

## Memory Management

- Static memory allocation for subscriptions
- Configurable maximum number of subscriptions
- Configurable maximum message size
- Automatic pipe queue management

## Platform Support

The system is designed to be platform-independent. Example implementations are provided for:
- FreeRTOS
- pthread

To support a new platform, implement the pipe interface functions:
- `pipe_init()`
- `pipe_delete()`
- `pipe_push()`
- `pipe_pop()`

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
