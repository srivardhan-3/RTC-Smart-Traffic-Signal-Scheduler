#ifndef _defines_h_
#define _defines_h_
#define SETBIT(word,bitpos) (word|=1<<bitpos)
#define SSETBIT(word,bitpos) (word=1<<bitpos)
#define CLRBIT(word,bitpos) (word&=1<<bitpos)
#define SCLRBIT(word,bitpos) SSETBIT(word,bitpos)
#define CPLBIT(word,bitpos) (word^=1<<bitpos)
#define READBIT(word,bitpos) ((word>>bitpos)&1)
#define WRITEBYTE(word,sbitpos,value) (word=(word&~(255<<sbitpos))|(value<<sbitpos))
#define WRITENIBBLE(word,sbitpos,nibble) (word=(word&~(15<<sbitpos))|(nibble<<sbitpos))
#define WRITEBIT(word,sbitpos,bit) ((word=word&~(1<sbitpos))|(bit<<sbitpos))
#define READNIBBLE(word,bitpos) ((word>>bitpos)&15)
#define READBYTE(word,sbitpos) ((word>>bitspos)&15)
#define READWRITEBYTE(word,sbitpos,dbit) ((word=(word&~(1<<dbit)|(word>>sbit)&1)<<dbit))
#endif
