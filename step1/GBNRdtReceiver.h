#include "Global.h"
class GBNRDTReceiver: public RdtReceiver{
    private:
    unsigned int base;
    Packet last_ackpkt;
    public:
    GBNRDTReceiver();
    ~GBNRDTReceiver();
    void receive(Packet &packet);		//���ձ��ģ�����NetworkService����	
};
