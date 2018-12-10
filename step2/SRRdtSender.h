#include "Global.h"
class SRRdtSender : public RdtSender{
    private:
    static const unsigned int K = 3;//3bit for seq
    static const unsigned int K_range=1<<K ; //K_range = 2^K
    static const unsigned int N = 4;//window_len = 4
    unsigned int base ;
    unsigned int nextnum ;
    unsigned int _base(){return base%K_range;};
    unsigned int _nextnum(){return nextnum%K_range;};
    unsigned int _nextcache(){return _nextnum()%N;};
    bool in_wds(int num);
    Packet pkt[N];
    bool ok[N];
    public:
    //���� ��������
    SRRdtSender();
    ~SRRdtSender();
    //�ӿں���
    bool send(Message &message) ;						//����Ӧ�ò�������Message����NetworkService����,������ͷ��ɹ��ؽ�Message���͵�����㣬����true;�����Ϊ���ͷ����ڵȴ�ȷ��״̬���ʹ����������ܾ�����Message���򷵻�false
    void receive(Packet &ackPkt) ;						//����ȷ��Ack������NetworkService����	
    void timeoutHandler(int seqNum) ;					//Timeout handler������NetworkService����
    bool getWaitingState() ;								//����RdtSender�Ƿ��ڵȴ�״̬��������ͷ����ȴ�ȷ�ϻ��߷��ʹ�������������true

};