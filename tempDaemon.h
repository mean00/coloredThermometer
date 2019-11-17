

#define TEMP_SAMPLING_COUNT 8

class TemperatureDaemon
{
public:
                TemperatureDaemon();
        bool    init(int dhtAdress);
        float   getTemp();
static  void    trampoline(void *);

protected:
        void  run();
        float _temp;
        float _measure[TEMP_SAMPLING_COUNT];
        bool  _first;
        int   _index;
  
};