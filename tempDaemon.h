


class TemperatureDaemon
{
public:
                TemperatureDaemon();
        bool    init();
        float   getTemp();
static  void    trampoline(void *);

protected:
        void  run();
        float _temp;
        
  
};