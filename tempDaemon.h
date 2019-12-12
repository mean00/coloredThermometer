


class TemperatureDaemon
{
public:
                TemperatureDaemon();
        bool    init(int beta);
        float   getTemp();
static  void    trampoline(void *);

protected:
        void  run();
        float _temp;
        
  
};