
Inter-Process Communication (IPC) provides different methods that allow processes to communicate, exchange data, or synchronize their activities.

1. Pipes
 Communication Model

Pipes provide one-way communication between processes. Data written by one process can be read by another process.

 Synchronization Requirements

Basic synchronization is handled by the operating system, but processes may need additional synchronization depending on the application.

Typical Use Cases

* Communication between related processes
* Sending output from one process to another
* Command-line operations

 Advantages

* Simple to use
* Easy to implement
* Suitable for small amounts of data

 Disadvantages

* Usually supports one-way communication
* Generally used between related processes
* Not suitable for large or complex communication



2. Named Pipes / FIFOs

Communication Model

Named pipes provide communication through a **named communication channel**. Unlike ordinary pipes, unrelated processes can also communicate using them.

Synchronization Requirements

The operating system manages the basic reading and writing operations, but processes may need additional synchronization.

Typical Use Cases

* Communication between unrelated processes
* Local client-server communication
* Simple data transfer between applications

 Advantages

* Can be used by unrelated processes
* Simple communication mechanism
* Easy to implement

 Disadvantages

* Mainly designed for communication on the same system
* Data transfer is generally sequential
* Less flexible than sockets

---

 3. Message Queues

 Communication Model

Message queues allow processes to communicate by **sending and receiving messages** through a queue maintained by the operating system.

 Synchronization Requirements

The operating system manages the queue and controls access to messages. Processes may still require synchronization for more complex operations.

 Typical Use Cases

* Sending structured messages between processes
* Client-server communication
* Task and job management

 Advantages

* Messages can be stored until the receiving process is ready
* Supports structured communication
* Processes do not need to communicate at exactly the same time

 Disadvantages

* Limited queue size
* More overhead than shared memory
* Messages may need to be copied



 4. Shared Memory

 Communication Model

Shared memory allows two or more processes to access a **common area of memory** for exchanging data.

 Synchronization Requirements

Synchronization is usually required to prevent multiple processes from accessing or modifying shared data incorrectly at the same time. Semaphores or other synchronization mechanisms can be used.

Typical Use Cases

* Sharing large amounts of data
* High-speed communication
* Multimedia and data-processing applications

Advantages

* Very fast communication
* Suitable for large amounts of data
* Reduces unnecessary data copying
* Multiple processes can access the same data

Disadvantages

* Requires synchronization
* Race conditions can occur without proper control
* More difficult to manage than pipes or message queues


 5. Signals

 Communication Model

Signals provide event notification to a process. A signal informs a process that a particular event or condition has occurred.

Synchronization Requirements

Signals do not normally provide synchronization for sharing data. They are mainly used for notification and process control.

Typical Use Cases

* Interrupting a process
* Terminating a process
* Handling system events
* Process control

 Advantages

* Simple and lightweight
* Useful for event notification
* Requires little communication overhead
* Useful for controlling processes

 Disadvantages

* Not suitable for transferring large amounts of data
* Provides limited information
* Signal handling can be difficult in complex programs



 6. Sockets

Communication Model

Sockets provide a **communication endpoint** through which processes can exchange data. They can communicate locally or across different computers through a network.

Synchronization Requirements

Synchronization depends on the socket type and application. Network communication may require connection management and appropriate protocols.

 Typical Use Cases

* Web applications
* Chat applications
* Online games
* File transfer
* Client-server applications

 Advantages

* Can communicate across different computers
* Supports large amounts of data
* Suitable for network applications
* Supports TCP and UDP communication

 Disadvantages

* More complex to implement
* Network communication can introduce delays
* Requires management of connections and errors



 7. Semaphores

Communication Model

Semaphores are mainly a synchronization mechanism rather than a data-transfer mechanism. They control access to shared resources.

Synchronization Requirements

Synchronization is the main purpose of semaphores. A semaphore uses a counter to control how many processes can access a resource.

 Typical Use Cases

* Controlling access to shared memory
* Managing shared resources
* Preventing race conditions
* Coordinating processes

Advantages

* Prevents conflicts between processes
* Helps avoid race conditions
* Controls access to shared resources
* Supports synchronization between processes

Disadvantages

* Does not directly transfer data
* Incorrect use can cause deadlocks
* Requires careful programming



 8. Memory-Mapped Files

Communication Model

Memory-mapped files allow a file to be mapped into a process's memory space. Multiple processes can map the same file and use it to share data.

 Synchronization Requirements

Synchronization may be required when multiple processes read or modify the mapped data at the same time.

 Typical Use Cases

* Sharing large data between processes
* File-based data sharing
* High-performance data processing

Advantages

* Efficient for large files
* Processes can access file data as memory
* Can reduce unnecessary copying
* Useful for sharing persistent data

 Disadvantages

* Requires synchronization for concurrent access
* More complex to manage
* Performance depends on memory and storage operations



 Comparison of IPC Mechanisms

 1. Pipes

* Communication: One-way data communication
     .Synchronization: Basic OS support
* Used for:Communication between related processes
* Advantage: Simple and easy to use
* Disadvantage: Limited and usually one-way

2. Named Pipes / FIFOs

* Communication: Data communication through a named channel
* Synchronization: Basic OS support
* Used for: Communication between unrelated processes
* Advantage: Simple and can connect unrelated processes
* Disadvantage: Mainly used on the same computer

 3. Message Queues

* Communication:Message-based communication
* Synchronization: Managed by the operating system
* Used for: Sending structured messages between processes
* Advantage: Messages can wait in the queue until received
* Disadvantage: Limited queue size and copying overhead

 4. Shared Memory

     .Communication: Processes share a common memory area
* Synchronization: Required to avoid conflicts
* Used for: Fast sharing of large amounts of data
* Advantage: Very fast communication
* Disadvantage: Requires proper synchronization

 5. Signals

* Communication: Event notification
* Synchronization: Mainly used for notification and process control
* Used for: Interrupting or controlling processes
* Advantage: Simple and lightweight
* Disadvantage: Not suitable for transferring large amounts of data

 6. Sockets

* Communication: Data communication between processes
* Synchronization: Depends on the application
* Used for: Network and client-server applications
* Advantage: Can communicate across different computers
* Disadvantage: More complex to implement

 7. Semaphores

* Communication: Resource synchronization
* Synchronization: Main purpose of the mechanism
* Used for: Controlling access to shared resources
* Advantage: Prevents conflicts and race conditions
* Disadvantage: Does not directly transfer data

 8. Memory-Mapped Files

* Communication: Data sharing through mapped files
* Synchronization: Usually required
* Used for:Sharing large amounts of file-based data
* Advantage: Efficient for large data
* Disadvantage:Requires careful synchronization and management

Overall Difference

* Pipes → Simple data transfer
* Named Pipes → Data transfer between unrelated local processes
* Message Queues → Structured message transfer
* Shared Memory → Fast data sharing
   . . Signals → Event notification
* Sockets →Local or network communication
* Semaphores →Synchronization and resource control
* Memory-Mapped Files → Sharing file data through memory

Advantages and Disadvantages – Quick Comparison
Pipes

Advantages: Simple, easy to implement
Disadvantages: Limited flexibility, mainly one-way communication

Named Pipes / FIFOs

Advantages: Can communicate between unrelated local processes
Disadvantages: Mainly limited to the same system

Message Queues

Advantages: Structured messages, asynchronous communication
Disadvantages: Limited queue size and copying overhead

Shared Memory

Advantages: Very fast, suitable for large data
Disadvantages: Requires synchronization

Signals

Advantages: Simple and lightweight notification
Disadvantages: Not suitable for transferring large amounts of data

Sockets

Advantages: Supports local and network communication
Disadvantages: More complex and may have network overhead

Semaphores

Advantages: Prevents conflicts and race conditions
Disadvantages: Does not transfer data directly; incorrect use can cause deadlocks

Memory-Mapped Files

Advantages: Efficient for large data and file sharing
Disadvantages: Requires synchronization and careful management

