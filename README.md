# binary-protocol-parser
This is a binary protocol parser that receives data packets and stores each payload in a fixed 16-byte buffer, rejecting damaged packets with a checksum.

## Packet format
EXAMPLE: AA 55 03 0A 14 1E 03
Start marker: AA 55
Length: 03 (3 payload bytes, 16 at most)
Payload(Data): 0A 14 1E
Check byte: 03

The check byte is the XOR of the length and every payload byte. The start marker is not covered.


## How it works
My parser reads the stream one byte at a time and moves through five states: it looks for AA, then 55, then reads the length, then the payload, then the check byte. If something is wrong (a bad marker byte, a length over 16, or a check that doesn't match), it goes back to looking for AA. A repeated AA doesn't break it, it just keeps waiting for 55.

While the bytes come in, it keeps a running check value. The payload goes into a fixed 16-byte buffer, so there is no heap allocation.

After every byte, feed tells the caller what happened: Pending if the packet is still arriving, Packet if a good one just finished, and Rejected if the length is over 16 or the check byte is wrong.


## Tests
run_streams runs 31 checks. Each one feeds a byte stream to the parser and compares the number of accepted packets with what I worked out on paper first. If any check fails, the program exits with a nonzero code, which is what CI reads.

The streams cover length edges, a flipped bit in each field, noise, recovery after a bad packet, and truncation. Two of them document limits: XOR accepts a packet with the same bit flipped in a data byte and in the check byte, and a cut-short packet can swallow the good one behind it.


## Sanitizers and CI
Every push runs the build on Linux through GitHub Actions, with AddressSanitizer and UBSan turned on and set to stop the program at the first report. The 31 checks run under both, so a memory error or undefined behavior fails the job. All 11 runs so far are green.

I don't run ASan locally because it hangs on my Mac, so CI is where it runs. UBSan also works on my machine. CI builds the XOR version only, so the CRC-8 build is checked by hand with pair_flips.


## XOR vs CRC-8
I flipped every pair of bits in a packet with a 4-byte payload (1128 pairs, start marker left alone) and counted how many damaged packets the parser still accepted. With XOR it accepted 81. With CRC-8 it accepted 1.

The one CRC-8 accepts is bit 2 of the length plus bit 4 of the first data byte. That turns the packet into a valid one with an empty payload, so the parser accepts it. No checksum would stop that.


## Limitations
A packet that ends early can swallow the good packet behind it (stream H in the tests).

I only tested double flips on one packet size and one polynomial. The tests and CI use XOR, and I checked CRC-8 by hand.
The soak test is 30 seconds on one USB cable, so it says little about a noisy link.


## Build and run
```
cmake -S . -B build
cmake --build build
./build/run_streams
```
For the CRC-8 build: cmake -S . -B build/crc8 -DUSE_CRC8=ON, then build it and run ./build/crc8/pair_flips.



## Soak test

I made an ESP32 send a packet every 10 ms over USB serial at 115200 baud: AA 55, length 16, a 16-byte payload with a counter, and the XOR check byte. I recorded 30 seconds (3379 packets, 67584 bytes) and ran the file through the parser with decode.cpp. All 3379 packets were accepted and none were rejected. With no errors in about 540,000 bits, the rule of three puts the bit-error rate below roughly 5.5e-6 at 95 percent confidence.

To check that the decoder actually counts errors, I damaged a copy of the recording. Changing a payload byte gave one rejected packet. Changing a start marker byte made one packet disappear without a rejection, so a lower good count is also a sign of damage.