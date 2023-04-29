#pragma once
#ifndef _circular_buf_h
#define _circular_buf_h

#include <Arduino.h>

class circular_bufC {
   public:
#define bufIndexBits 4  // 2^4 gives the 16 for bufLength. // if changed this needs to be  2^x ie 2,4,8,16...265. that way div can use shift and mod is just anding.

#define bufLength (1 << bufIndexBits)  // 2^4 = 16

#if ((bufLength) & (bufLength - 1)) != 0
    #error "The 'bufLength' of the circular buffer defined in 'circular_buf.h' must be a power of two"
#endif

#define bufMsk (bufIndexBits - 1)  // B1111 Mask to give the remainder. In case of buf size of 8 = 2^3 then bufMsk = B00000111 = 7
#define OutOfSpace 1
#define ReadEmptyBuf 2

    /**
     * @brief return the index inside the buffer, higher values wraparound.
     *
     * i(the index) is relative to the array used the store the buffer, not the buffer i.e relative to the head pointer.
     *
     * @param i index, can be out of range(to big).
     * @return byte The index inside the buffer.
     */
    inline byte bufIndex(byte i) { return i bitand bufMsk; }
    inline byte nextIndex() { return bufIndex(headP + lengthB); }                  // Index of next free buffer pos, if buffer is full this will be the head index.
    inline byte nextIndex(byte plus) { return bufIndex(headP + lengthB + plus); }  // As nextIndex() but add 'plus' it the index.

    inline byte peek() { return buff[headP]; }                      /// returns the value at the head of the circular buffer.
    inline byte peek(byte x) { return buff[bufIndex(x + headP)]; }  /// return the value at the head + x of the circular buffer.

    byte* bufP(byte i) { return &buff[i]; }

    /// @brief returns the value of the the the array the buffer is stored in at element i.
    /// @param i the index into the underlying array.
    /// @return
    byte getBufArrayElement(byte i) { return buff[bufIndex(i)]; }

    //    void enqueue(byte x);
    //    byte dequeue();
    void push(byte x) {
        if (lengthB < bufLength) {
            buff[nextIndex()] = x;
            lengthB++;
        } else {
            error = OutOfSpace;
        }
    }
    byte pull() {
        byte r;
        if (lengthB > 0) {
            r = buff[headP];
            headP = bufIndex(headP + 1);
            lengthB--;
            return r;
        }
        // when called on empty buffer return 0 but this could also be a value so check before calling.
        error = ReadEmptyBuf;
        return 0;
    };  // fifo, get the first byte of the buffer from the head and delete it.
    byte len() { return lengthB; }
    const inline byte maxLen() { return bufLength; }
    byte space() { return bufLength - lengthB; }
    byte getLength() { return lengthB; }
    void setLength(byte x) {
        if (x < bufLength) lengthB = x;
    }

   private:
    byte buff[bufLength];  // 2^x so can use shift for math.
    byte headP = 0;
    byte lengthB = 0;
    byte error = 0;  // 1:out of space, 2:read empty buffer
};

#endif