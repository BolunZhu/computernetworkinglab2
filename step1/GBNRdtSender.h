#include "Global.h"
class GBNRdtSender : public RdtSender{
    private:
    static const unsigned int K = 3;//3bit for seq
    static const unsigned int N = 4;//window_len = 4
    unsigned int base ;
    unsigned int nextnum ;
    unsigned int _base(){return base&0x7;};
    unsigned int _nextnum(){return nextnum&0x7;};
    Message msg[N];
    public:
    //���� ��������
    GBNRdtSender();
    ~GBNRdtSender();
    //�ӿں���
    bool send(Message &message) ;						//����Ӧ�ò�������Message����NetworkService����,������ͷ��ɹ��ؽ�Message���͵�����㣬����true;�����Ϊ���ͷ����ڵȴ�ȷ��״̬���ʹ����������ܾ�����Message���򷵻�false
    void receive(Packet &ackPkt) ;						//����ȷ��Ack������NetworkService����	
    void timeoutHandler(int seqNum) ;					//Timeout handler������NetworkService����
    bool getWaitingState() ;								//����RdtSender�Ƿ��ڵȴ�״̬��������ͷ����ȴ�ȷ�ϻ��߷��ʹ�������������true

    //

};
GBNRdtSender::GBNRdtSender():base(0),nextnum(0){

}
bool GBNRdtSender::send(Message &message) {
    if(_nextnum()<_base()||_nextnum()>=_base()+N){
        return false;
    }
    Packet pkt;
    pkt.acknum=base;
    pkt.checksum=pUtils->calculateCheckSum(pkt);
    pkt.seqnum=_nextnum();
    pns->sendToNetworkLayer(SENDER,pkt);
}