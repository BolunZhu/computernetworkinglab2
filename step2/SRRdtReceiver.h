#include "Global.h"
class SRRDTReceiver: public RdtReceiver{
    private:
    unsigned int base;
    static const unsigned int N=4;
    static const unsigned int K=3;
    static const unsigned int K_range=1<<K;
    Packet last_ackpkt;
    void send_ackpkt(int seqnum);
    unsigned int _base(){return base%K_range;};
    public:
    SRRDTReceiver();
    ~SRRDTReceiver();
    void receive(Packet &packet);		//���ձ��ģ�����NetworkService����	
};
