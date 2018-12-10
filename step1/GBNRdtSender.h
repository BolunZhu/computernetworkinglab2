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

};