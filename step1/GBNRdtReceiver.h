#include "Global.h"
class GBNRDTReceiver: public RdtReceiver{
    private:
    unsigned int base;
    Packet last_ackpkt;
    public:
    GBNRDTReceiver();
    ~GBNRDTReceiver();
    void receive(Packet &packet);		//接收报文，将被NetworkService调用	
};
