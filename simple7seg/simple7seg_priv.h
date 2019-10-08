#include "simple7seg.h"
/**
 */
class simple7SegImpl : public simple7Seg
{
public:    
             simple7SegImpl(int D1, int D2, int D3, int D4, int aa, int bb, int cc, int dd, int ee, int ff, int gg, int decimalpoint);         
        bool setNumber(int digit, int value);
        bool printAsFloat(float f);
        bool setSignificantDigits(int j);
protected:
        int _digits[4];
        int _pins[10];
        int _value[4];
        int _staging[4];
        bool _update;
        float _mul;
        int _significant;
        void run();
 static void trampoline(void *a);
        
};
// EOF
