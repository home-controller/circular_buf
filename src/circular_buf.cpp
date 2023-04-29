/**
 * @brief This is no longer used as moved all to the header file the typedefs are all part of the class.
 * 
 */

/*
 typedef struct {
    uint8_t * const buffer;
    int head;
    int tail;
    const int maxlen;
} circ_bbuf_t;

typedef struct {
    uint8_t * const buffer;
    int head;
    int tail;
    const int maxlen;
} c_buf8_t;

#define _bufSize 8 //2^3    // if changed this needs to be  2^x ie 2,4,8,16...265. that way div can use shift and mod is just anding.
#define _bufSizeM _bufSize - 1 // _bufSizeM = Mask to give the remainder. in case of buf size of 8 = 2^3 then _bufSizeM = B00000111 = 7
class circular_buf{
  public:
//    void enqueue(byte x);
//    byte dequeue();
    void push(byte x);
    byte pull();// fifo, get the first byte of the buffer from the head and delete it.

  private:

    byte      _buffer[_bufSize];
    byte      _bufHeadP =0;
    byte      _BufLen =0;
};
byte circular_buf::pull()// fifo, takes the head of the buffer deletes it from the buffer and returns it.
{
  if(_BufLen > 0){
    byte t = _bufHeadP;
    //t = t & _bufSizeM; // for _bufSize = 8 whe have: t mod 8 = t mod B00001000 = t & B00000111 so this is equivalent to t=tmod8
    t = _buffer[t];
    _bufHeadP++;
    _bufHeadP=_bufHeadP&_bufSizeM;
    _BufLen--;
    return t;
  }
  // when called on empty buffer return 0 but this could also be a value so check before calling.
  return 0;
}
#define _bpush(buf, buf_l, buf_h)
void circular_buf::push(byte x)// if used in a interrupt should inline so no int flag stays set? convert body to macro func?
{
  if(_BufLen < _bufSize){
    byte t = _bufHeadP + _BufLen;
    t = t & _bufSizeM; // for _bufSize = 8 we have: t mod 8 = t mod B00001000 = t & B00000111 so this is equivalent to t = t mod 8
    _buffer[t] = x; //(_bufHeadP + _BufLen) % (_bufSize-1)];
    _BufLen++;
  }
}
 */