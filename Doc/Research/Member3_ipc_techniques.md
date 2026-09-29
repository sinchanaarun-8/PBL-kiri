# IPC Techniques and Communication Models

**Project:** PBL-kiri  
**Research Area:** Inter-Process Communication (IPC)  
**Research Member:** Member 03 **Aahil**

## 1. Introduction

IPC techniques extend basic process communication by providing event notification, synchronization, local or network communication, and specialized data-sharing mechanisms.

This section covers techniques such as signals, sockets, semaphores, and memory-mapped files, followed by common IPC communication models and design considerations.

---

## 2. Signals

### Communication Model

Signals provide event notification to a process. A signal informs a process that a particular event or condition has occurred.

### Synchronization Requirements

Signals do not normally provide synchronization for sharing data. They are mainly used for notification and process control.

### Typical Use Cases

- Interrupting a process
- Terminating a process
- Handling system events
- Process control

### Advantages

- Simple and lightweight
- Useful for event notification
- Requires little communication overhead
- Useful for controlling processes

### Disadvantages

- Not suitable for transferring large amounts of data
- Provides limited information
- Signal handling can be difficult in complex programs

---

## 3. Sockets

### Communication Model

Sockets provide a communication endpoint through which processes can exchange data. They can communicate locally or across different computers through a network.

### Synchronization Requirements

Synchronization depends on the socket type and application. Network communication may require connection management and appropriate protocols.

### Typical Use Cases

- Web applications
- Chat applications
- Online games
- File transfer
- Client-server applications

### Advantages

- Can communicate across different computers
- Supports large amounts of data
- Suitable for network applications
- Supports TCP and UDP communication

### Disadvantages

- More complex to implement
- Network communication can introduce delays
- Requires management of connections and errors

---

## 4. Semaphores

### Communication Model

Semaphores are mainly a synchronization mechanism rather than a data-transfer mechanism. They control access to shared resources.

### Synchronization Requirements

Synchronization is the main purpose of semaphores. A semaphore uses a counter to control how many processes can access a resource.

### Typical Use Cases

- Controlling access to shared memory
- Managing shared resources
- Preventing race conditions
- Coordinating processes

### Advantages

- Prevents conflicts between processes
- Helps avoid race conditions
- Controls access to shared resources
- Supports synchronization between processes

### Disadvantages

- Does not directly transfer data
- Incorrect use can cause deadlocks
- Requires careful programming

---

## 5. Memory-Mapped Files

### Communication Model

Memory-mapped files allow a file to be mapped into a process's memory space. Multiple processes can map the same file and use it to share data.

### Synchronization Requirements

Synchronization may be required when multiple processes read or modify the mapped data at the same time.

### Typical Use Cases

- Sharing large data between processes
- File-based data sharing
- High-performance data processing

### Advantages

- Efficient for large files
- Processes can access file data as memory
- Can reduce unnecessary copying
- Useful for sharing persistent data

### Disadvantages

- Requires synchronization for concurrent access
- More complex to manage
- Performance depends on memory and storage operations

---

## 6. Message-Passing Model

In message passing, one process sends a message and another process receives it.

```text
+-------------+       Message       +-------------+
|  Process A  | ------------------> |  Process B  |
+-------------+                     +-------------+
```

The communication mechanism manages the communication channel.

Examples include:

- Pipes
- Message queues
- Sockets

Message passing is useful when processes need to exchange commands, events, results, or other discrete information.

---

## 7. Shared-Memory Model

In shared memory, two or more processes access a common memory region.

```text
        +----------------------+
        |     Shared Memory    |
        |                      |
        |      Shared Data     |
        +----------+-----------+
                   ^
              +----+----+
              |         |
        +-----+---+ +---+-----+
        |Process A| |Process B|
        +---------+ +---------+
```

This model is useful when processes need to exchange large amounts of local data.

However, proper synchronization is required when multiple processes access or modify the same data.

---

## 8. Client-Server Model

IPC can also be organized using a client-server model.

The client sends a request to the server, and the server sends back a response.

```text
+-------------+                    +-------------+
|   Client    | ---- Request ----> |   Server    |
|   Process   | <--- Response ---- |   Process   |
+-------------+                    +-------------+
```

This model is commonly used when one process provides a service and another process requests that service.

Sockets are commonly used for client-server communication, although other IPC mechanisms can also be used for suitable local designs.

---

## 9. Producer-Consumer Model

Another common IPC design is the producer-consumer model.

```text
+-------------+
|  Producer   |
+------+------+
       |
       | Data
       v
+-------------+
| IPC Buffer  |
| / Queue     |
+------+------+
       |
       | Data
       v
+-------------+
|  Consumer   |
+-------------+
```

The producer creates data or work items, while the consumer processes them.

A queue or shared memory can be used depending on the requirements.

---

## 10. User Space and Kernel Space

Applications normally run in user space. The operating system kernel runs with higher privileges and manages important system resources.

A simplified model is:

```text
+---------------------------+
|       Application         |
|        Process A          |
+-------------+-------------+
              |
              | System Call / API
              v
+---------------------------+
|       Operating System    |
|          Kernel           |
+-------------+-------------+
              |
              v
+---------------------------+
|       IPC Resource        |
+-------------+-------------+
              |
              v
+---------------------------+
|        Process B          |
+---------------------------+
```

The exact implementation differs between operating systems.

---

## 11. Synchronization

Communication alone is not always enough.

If two processes access the same resource at the same time, problems can occur.

For example:

```text
Process A → Read data
Process B → Read same data
Process A → Modify data
Process B → Modify data
```

The final result may become incorrect if the operations are not properly coordinated.

This is called a race condition.

Synchronization mechanisms such as semaphores, mutexes, or other platform-supported mechanisms can be used to control access to shared resources.

---

## 12. IPC Resource Lifecycle

IPC resources generally follow a lifecycle:

```text
Create / Establish
        |
        v
Open / Attach
        |
        v
Communicate
        |
        v
Close / Detach
        |
        v
Release / Destroy
```

The exact steps depend on the IPC mechanism.

Proper resource management helps avoid resource leaks and unexpected behavior.

---

## 13. Process Failure

An IPC architecture should consider what happens if one process stops unexpectedly.

For example:

```text
Process A -------- IPC --------> Process B
                                  X
                            Process B stops
```

Process A may receive an error, closed connection, end-of-file condition, or another indication depending on the IPC mechanism.

Therefore, applications should not assume that the other process will always remain available.

---

## 14. Security Considerations

IPC resources should be protected from unauthorized access.

Important security considerations include:

- Controlling who can access the IPC resource
- Using operating-system permissions where available
- Validating data received from another process
- Avoiding unnecessary exposure of sensitive information
- Handling excessive requests or resource usage
- Using authentication and encryption when required for network-based communication

A local process should not automatically be trusted just because it is running on the same computer.

---

## 15. Local IPC and Network Communication

IPC can be used for communication between processes on the same computer. Some mechanisms, especially sockets, can also support communication between different computers.

| Local IPC | Network Communication |
|---|---|
| Usually same computer | Can involve different computers |
| Pipes | Network sockets |
| FIFOs | TCP/UDP sockets |
| Shared memory | Application protocols |
| Local sockets | Network-based client-server systems |

The required communication environment should be considered before selecting an IPC mechanism.

---

## 16. Comparison of IPC Techniques

| Technique | Primary Purpose | Typical Use |
|---|---|---|
| Signals | Event notification | Process control and events |
| Sockets | Process/network communication | Client-server and network applications |
| Semaphores | Synchronization | Resource access control |
| Memory-Mapped Files | Data sharing | Large file-based data |

---

## 17. Important Design Questions

Before implementing an IPC system, the following questions should be considered:

1. Which processes need to communicate?
2. What data needs to be exchanged?
3. Is the communication local or network-based?
4. Is the data a stream or separate messages?
5. Is shared memory required?
6. Is synchronization required?
7. What happens if one process stops?
8. Who is allowed to access the IPC resource?
9. How will errors be handled?
10. Which IPC method or technique best matches the requirements?

---

## 18. Conclusion

IPC can be understood through both communication methods and supporting techniques.

The primary IPC methods include:

- Pipes
- Named Pipes / FIFOs
- Message Queues
- Shared Memory

Supporting IPC techniques include:

- Signals
- Sockets
- Semaphores
- Memory-Mapped Files

Common communication models include:

- Message Passing
- Shared Memory
- Client-Server
- Producer-Consumer

A suitable IPC design should consider communication requirements, synchronization, process failure, security, resource management, and whether communication is local or network-based.
