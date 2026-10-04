# Investigation of Endianness

**Author:** Farida Shakikhanli

## 1. Definition

Computers store both data and machine-code instructions in memory as bits (0s and 1s), grouped into bytes. When a value occupies more than one byte, the system needs a convention for arranging those bytes in memory. Endianness refers to this byte order. Big-endian stores the most significant byte at the lowest memory address, while little-endian stores the least significant byte at the lowest memory address. 

## 2. Big-Endian and Little-Endian

- **Big-endian:** Stores the most significant byte (MSB) at the lowest memory address. MSB is byte that holds the highest position value. For example,  in 3248, it is 3.
- **Little-endian:** Stores the least significant byte (LSB) at the lowest memory address.LSB is byte that has the lowest position value.For example,  in 3248, it is 8.

For example, the 32-bit integer `0x12345678`(represented in hexadecinmal) occupies four bytes.
The table shows these bytes in order of increasing memory address:

| Byte order | Address 1000 | Address 1001 | Address 1002 | Address 1003 |
|------------|-------------|-------------|-------------|-------------|
| Big-endian | `12`        | `34`        | `56`        | `78`        |
| Little-endian | `78`     | `56`        | `34`        | `12`        |

Both arrangements represent the same numerical value when read using the correct byte order. Using the wrong byte order causes the bytes to be interpreted as a different value.

## 3. Why Endianness Matters

[Discuss exchanging binary data through files or networks.
Explain what can happen when the sender and receiver assume different byte orders.]
Endianness must be considered when data is shared between two systems with different byte orders.
For example, sender computer's cpu is decoded with little endian configuration.
A sender encodes the 16-bit number 1 in little-endian: 01 00.
The receiver assumes big-endian and reads those bytes as 0x0100, which is 256.

The bytes arrive correctly, but the receiver gives them the wrong meaning. If that number represents a message length, for example, the receiver could expect 256 bytes instead of 1.
The solution is to agree on the byte order for the transmitted data and encode/decode accordingly.
## 4. Critical Evaluation
In my view, neither big-endian nor little-endian is universally better. Big-endian is easier for humans to read in a hexadecimal memory dump because the bytes appear in the same order as the written number. However, readability alone does not make it technically superior. For compatibility, the important factor is that systems agree on how to interpret the bytes. File formats and communication protocols should explicitly specify byte order rather than assume that every computer uses the same convention. This prevents incorrect values and makes data exchange more reliable.

## 5. References

- [UC Berkeley CS61C — Words and Endianness](https://notes.cs61c.org/content/c-odds-and-ends/endianness/)
- [GNU C Library — Byte Order](https://sourceware.org/glibc/manual/2.44/html_node/Byte-Order.html)
- [GeeksForGeeks — What is Endiannes? ](https://www.geeksforgeeks.org/dsa/little-and-big-endian-mystery/)