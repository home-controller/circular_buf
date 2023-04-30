/**
 * @file circular_buf.h
 * @author jmnc2 (you@domain.com)
 * @brief A circular buffer for MCUs that can be used in an interrupt handler, so it needs to be small and fast.
 * @version 0.1
 * @date 2023-04-30
 *
 * @copyright Copyright (c) 2023
 *
 * @details Because this needs to be small and fast the buffer length must be a power of 2 to make the math easier.
 * This is set by the number of bits. So setting the number of bits form 1 to 8 then 2^bufIndexBits gives the buffer
 * lengths of [2,4,8,16,32,64,128,256]
 * The limit of the number of bits to 8 is because everything is stored in bytes(uint8_t), like the buffer length head pointer etc.
 *
 * If you want to change the number of bits #define bufIndexBits before the #include or set a compiler build flag e.g.:
 * build_flags =
 *      -D bufIndexBits=5
 * This works for platformIO buf you could also use ENV variables etc.
 *
 * But the simplest way is to just edit this file :D
 *
 * There is no setup for the class initialization code or setup.
 *
 * example:
 *
 */

#pragma once
#ifndef _circular_buf_h
#define _circular_buf_h

// #include <Arduino.h>

// using byte = std::uint8_t;
typedef uint8_t byte;

// error numbers
#define OutOfSpace 1
#define ReadEmptyBuf 2

class circular_bufC {
   public:
// #define bufIndexBits 4
#ifndef bufIndexBits
#define bufIndexBits 4  // 2^4 gives the 16 for bufLength. As we are using bytes it can be [1..8] to give a buffer length of [2,4,8,16,32,64,128,256]
#elif bufIndexBits < 1 || bufIndexBits > 8
#define bufIndexBits 4
#endif

// if bufLength needs to change it should be  2^x ie 2,4,8,16...265. that way div can use shift and mod is just anding.
/**
 * @brief the amount of memory in bytes used by the circular buffer(the max length).
 *
 */
#define bufLength (1 << bufIndexBits)  // 2^4 = 16

#if ((bufLength) & (bufLength - 1)) != 0
#error "The 'bufLength' of the circular buffer defined in 'circular_buf.h' must be a power of two"
#endif

#define bufMsk (bufLength - 1)  // B1111 Mask to give the remainder. In case of buf size of 8 = 2^3 then bufMsk = B00000111 = 7

    /**
     * @brief return the index inside the buffer, higher values wraparound.
     *
     * i(the index) is relative to the array used the store the buffer, not the buffer i.e relative to the head pointer.
     *
     * @param i index, can be out of range(to big).
     * @return byte The index inside the buffer.
     */
    inline byte bufIndex(byte i) { return i bitand bufMsk; }
    inline byte nextIndex() { return bufIndex((byte)headP + (byte)lengthB); }                                // Index of next free buffer pos, if buffer is full this will be the head index.
    inline byte nextIndex(byte x) { return bufIndex((byte)(headP + (byte)lengthB) + (byte)x); }  // As nextIndex() but add 'plus' it the index.

    inline byte peek() { return buff[headP]; }  /// returns the value at the head of the circular buffer.
    /// There is no range checking, Will still return value of the array at the head pointer + x even if the buffer is empty.
    inline byte peek(byte x) { return buff[bufIndex(x + headP)]; }  /// return the value at the head + x of the circular buffer.
    inline byte getLastError() { return error; }                    /// returns the last error if any.

    byte* bufP(byte i) { return &buff[i]; }

    /// @brief returns the value of the the the array the buffer is stored in at element i.
    /// @param i the index into the underlying array. 0 for the first element.
    /// @return
    byte getBufArrayElement(byte i) { return buff[bufIndex(i)]; }

    //    void enqueue(byte x);
    //    byte dequeue();
    void push(byte x) {
        // this should be OK without locks or disabling interrupts I think?
        if (lengthB < bufLength) {
            buff[nextIndex()] = x;
            lengthB++;
        } else {
            error = OutOfSpace;
        }
    }
    /// @brief Takes the value at the head of the circular buffer and then deletes it.
    /// @return byte value that was deleted.
    byte pull() {
        byte r;
        if (lengthB > 0) {
            r = buff[headP];
            // TODO: Should interrupts be disabled here?
            headP = bufIndex(headP + 1);
            lengthB--;
            // TODO: And enabled again here?
            return r;
        }
        // when called on empty buffer return 0 but this could also be a value so check before calling.
        error = ReadEmptyBuf;
        return 0;
    };  // fifo, get the first byte of the buffer from the head and delete it.
    // Alias for getLength().
    byte len() { return lengthB; }
    inline byte maxLen() { return bufLength; }
    /// @return byte, unused space in the buffer.
    byte space() { return bufLength - lengthB; }
    /// @brief gets the number of values stored in the buffer.
    byte getLength() { return lengthB; }

    /// @brief Cap number of elements stored in buffer currently(delete extra from end).
    /// @param x Max buffer elemets.
    void setLength(byte x) {
        if (x < bufLength) lengthB = x;
    }
    byte getHeadP() { return headP; };//should oly need this for debugging.

    private : byte buff[bufLength];  // 2^x so can use shift for math.
    byte headP = 0;
    byte lengthB = 0;
    byte error = 0;  // 1:out of space, 2:read empty buffer
};

#endif
