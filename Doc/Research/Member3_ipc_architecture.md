# IPC Architecture - Aahil (Member 03)

**Project:** PBL-kiri 
**Research Area:** Inter-Process Communication (IPC)  
**Research Member:** Member 03

---

## 1. Introduction

IPC Architecture explains **how different processes communicate with each other inside a computer system**.

A process normally has its own memory space. Therefore, one process cannot simply access another process's private memory. The operating system provides IPC mechanisms that allow processes to exchange data and coordinate their work.

A simple view is:

```text
Process A
    |
    v
   IPC
    |
    v
Process B
```

The exact IPC mechanism can be a pipe, message queue, shared memory, socket, or another supported mechanism.

---

## 2. Basic IPC Architecture

The basic architecture contains three main parts:

```text
+-------------+          +-------------+
|  Process A  |          |  Process B  |
+------+------+          +------+------+
       |                        ^
       |                        |
       v                        |
+-----------------------------------------+
|              IPC Mechanism              |
|   Pipe / Queue / Shared Memory / Socket |
+-----------------------------------------+
```

Process A sends or shares information through an IPC mechanism, and Process B receives or accesses that information.

The operating system manages or provides the required resources depending on the IPC mechanism.

---

## 3. User Space and Kernel Space

Applications normally run in **user space**. The operating system kernel runs with higher privileges and manages important system resources.

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

## 4. Message-Passing Architecture

In message passing, one process sends a message and another process receives it.

```text
+-------------+       Message       +-------------+
|  Process A  | ------------------> |  Process B  |
+-------------+                     +-------------+
```

The IPC mechanism manages the communication channel.

Examples include:

- Pipes
- Message queues
- Sockets

Message passing is useful when processes need to exchange commands, events, results, or other discrete information.

---

## 5. Shared-Memory Architecture

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

This can be useful when processes need to exchange a large amount of local data.

However, shared memory requires proper synchronization when multiple processes access or modify the same data.

---

## 6. Client-Server IPC Architecture

IPC can also be used in a client-server model.

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

## 7. Producer-Consumer Architecture

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

## 8. Synchronization

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

This is called a **race condition**.

Synchronization mechanisms such as semaphores, mutexes, or other platform-supported mechanisms can be used to control access to shared resources.

---

## 9. IPC Resource Lifecycle

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

For example, a process may create or open a communication endpoint, use it, and then close or release it when communication is finished.

Proper resource management helps avoid resource leaks and unexpected behavior.

---

## 10. Process Failure

An IPC architecture should also consider what happens if one process stops unexpectedly.

For example:

```text
Process A  -------- IPC -------->  Process B
                                      X
                                Process B stops
```

Process A may receive an error, closed connection, end-of-file condition, or another indication depending on the IPC mechanism.

Therefore, the application should not assume that the other process will always remain available.

---

## 11. Security

IPC resources should be protected from unauthorized access.

Important security considerations include:

- Controlling who can access the IPC resource.
- Using operating-system permissions where available.
- Validating data received from another process.
- Avoiding unnecessary exposure of sensitive information.
- Handling excessive requests or resource usage.
- Using authentication and encryption when required for network-based communication.

A local process should not automatically be trusted just because it is running on the same computer.

---

## 12. Local IPC and Network IPC

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

## 13. Overall IPC Architecture

A general IPC architecture can be represented as:

```text
                  +----------------+
                  |   Process A    |
                  +-------+--------+
                          |
                          |
                          v
                  +---------------+
                  | IPC Mechanism |
                  +-------+-------+
                          |
                          |
                          v
                  +----------------+
                  |   Process B    |
                  +----------------+

                          |
                          v
                 Synchronization
                 Error Handling
                 Security
```

The actual architecture depends on the application's requirements.

---

## 14. Important Design Questions

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
10. Which IPC mechanism best matches the requirements?

---

## 15. Conclusion

IPC Architecture describes how processes, operating-system resources, communication mechanisms, and synchronization work together.

The basic models include:

- **Message passing** for exchanging messages.
- **Shared memory** for sharing data between processes.
- **Client-server architecture** for request and response communication.
- **Producer-consumer architecture** for transferring work or data.
- **Synchronization mechanisms** for safely coordinating processes.

A good IPC architecture should consider communication requirements, synchronization, process failure, security, and resource management before implementation.

The specific IPC method should be selected based on the requirements of the project rather than using the same mechanism for every situation.

---

