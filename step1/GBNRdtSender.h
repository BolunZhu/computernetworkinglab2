#include "Global.h"
#include "string"
class GBNRdtSender : public RdtSender{
    private:
    static const unsigned int K = 3;//3bit for seq
    static const unsigned int K_range=1<<K ; //K_range = 2^K
    static const unsigned int N = 4;//window_len = 4
    unsigned int base ;
    unsigned int nextnum ;
    unsigned int _base(){return base&(K_range-1);};
    unsigned int _nextnum(){return nextnum&(K_range-1);};
    unsigned int _nextcache(){return _nextnum()&(N-1);};
    Packet pkt[N];
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
GBNRdtSender::~GBNRdtSender(){}
bool GBNRdtSender::getWaitingState(){
    return _nextnum()==_base()+N;
}
bool GBNRdtSender::send(Message &message) {
    if(getWaitingState()){//full
        return false;
    }
    //_nextnum()&(N-1) = nextnum in N
    pkt[_nextcache()].acknum=-1;//ignore this
    pkt[_nextcache()].checksum=0;
    pkt[_nextcache()].seqnum=_nextnum();
    memcpy(pkt[_nextcache()].payload,message.data,sizeof(message.data));
    pkt[_nextcache()].checksum=pUtils->calculateCheckSum(pkt[_nextcache()]);
    
    pns->sendToNetworkLayer(SENDER,pkt[_nextcache()]);
    if(_base()==_nextnum())
    {
            pns->startTimer(SENDER,Configuration::TIME_OUT,0);
    }
    nextnum++;
    return true;
}
void GBNRdtSender::receive(Packet &ackPkt){
    int checksum = pUtils->calculateCheckSum(ackPkt);
    
    if (checksum == ackPkt.checksum && ackPkt.acknum >= _base() ) {
        base=ackPkt.acknum+1;
        pns->stopTimer(SENDER,0);
        //if all send out then stop timer
        //else restart timer
        if(_base()!=_nextnum()){
            pns->startTimer(SENDER,Configuration::TIME_OUT,0);
        }
    }
    else {
        // do nothing
    }
}
void GBNRdtSender::timeoutHandler(int seqNum){
    //time out then restart timer
    pns->stopTimer(SENDER,0);
    pns->startTimer(SENDER,Configuration::TIME_OUT,0);
    //resend all
    for(unsigned int i=_base();i!=_nextnum();i++){
        pns->sendToNetworkLayer(SENDER,pkt[i&(N-1)]);
    }
}
