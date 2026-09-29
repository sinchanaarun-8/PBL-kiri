
**Joshna**

# IPC Methods

**Project:** PBL-kiri  
**Research Area:** Inter-Process Communication (IPC)  
**Research Member:** Member 02

## 1. Introduction

Inter-Process Communication (IPC) provides different methods that allow processes to communicate, exchange data, and coordinate their activities.

This section focuses on the primary IPC methods used for direct data communication between processes.

---

## 2. Pipes

### Communication Model

Pipes provide one-way communication between processes. Data written by one process can be read by another process.

### Synchronization Requirements

Basic synchronization is handled by the operating system, but processes may need additional synchronization depending on the application.

### Typical Use Cases

- Communication between related processes
- Sending output from one process to another
- Command-line operations

### Advantages

- Simple to use
- Easy to implement
- Suitable for small amounts of data

### Disadvantages

- Usually supports one-way communication
- Generally used between related processes
- Not suitable for large or complex communication

---

## 3. Named Pipes / FIFOs

### Communication Model

Named pipes provide communication through a named communication channel. Unlike ordinary pipes, unrelated processes can also communicate using them.

### Synchronization Requirements

The operating system manages the basic reading and writing operations, but processes may need additional synchronization.

### Typical Use Cases

- Communication between unrelated processes
- Local client-server communication
- Simple data transfer between applications

### Advantages

- Can be used by unrelated processes
- Simple communication mechanism
- Easy to implement

### Disadvantages

- Mainly designed for communication on the same system
- Data transfer is generally sequential
- Less flexible than sockets

---

## 4. Message Queues

### Communication Model

Message queues allow processes to communicate by sending and receiving messages through a queue maintained by the operating system.

### Synchronization Requirements

The operating system manages the queue and controls access to messages. Processes may still require synchronization for more complex operations.

### Typical Use Cases

- Sending structured messages between processes
- Client-server communication
- Task and job management

### Advantages

- Messages can be stored until the receiving process is ready
- Supports structured communication
- Processes do not need to communicate at exactly the same time

### Disadvantages

- Limited queue size
- More overhead than shared memory
- Messages may need to be copied

---

## 5. Shared Memory

### Communication Model

Shared memory allows two or more processes to access a common area of memory for exchanging data.

### Synchronization Requirements

Synchronization is usually required to prevent multiple processes from accessing or modifying shared data incorrectly at the same time. Semaphores or other synchronization mechanisms can be used.

### Typical Use Cases

- Sharing large amounts of data
- High-speed communication
- Multimedia and data-processing applications

### Advantages

- Very fast communication
- Suitable for large amounts of data
- Reduces unnecessary data copying
- Multiple processes can access the same data

### Disadvantages

- Requires synchronization
- Race conditions can occur without proper control
- More difficult to manage than pipes or message queues

---

## 6. Comparison of IPC Methods

| Method | Communication | Typical Use | Main Advantage | Main Limitation
| Pipes | One-way data stream | Related processes | Simple | Usually one-way |
| Named Pipes / FIFOs | Named data channel | Unrelated local processes | Simple local communication | Mainly same system |
| Message Queues | Structured messages | Task and message exchange | Messages can wait in queue | Queue limits and copying overhead |
| Shared Memory | Common memory region | Large/high-speed data | Very fast | Requires synchronization |

---

## 7. Overall Difference

- **Pipes** → Simple data transfer
- **Named Pipes** → Data transfer between unrelated local processes
- **Message Queues** → Structured message transfer
- **Shared Memory** → Fast data sharing

These methods can be selected according to the amount and type of data, required communication pattern, performance requirements, and synchronization needs.
