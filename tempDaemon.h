


class TemperatureDaemon
{
public:
                TemperatureDaemon();
        bool    init(int beta, int resistance);
        float   getTemp();
        float   getResistance();
static  void    trampoline(void *);

protected:
        void  run();
        float _temp;
        float _resistance;
        
  
};