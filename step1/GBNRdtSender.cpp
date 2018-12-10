#include "stdafx.h"
#include "GBNRdtSender.h"

bool GBNRdtSender::in_wds(int num)
{  // base<num<base+N || num<base+N<base || base+N<base<num return true 
    unsigned int up = (_base()+N)%K_range;                                                 
    return (   (   up > _base() &&     
                    num>=_base() &&                     
                    num< up                     
                )//case: base+N>base 
                ||
                (   up < _base() && 
                    (   num>=_base() || 
                        num< up
                    )
                )//case: base+N<base 
            ); 
}
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
    //make & send packet tmp
    Packet tmp;
    tmp.acknum=-1;//ignore this
    tmp.checksum=0;
    tmp.seqnum=_nextnum();
    memcpy(tmp.payload,message.data,sizeof(message.data));
    tmp.checksum=pUtils->calculateCheckSum(tmp);
    //copy tmp in cache pkt[N]
    pkt[_nextcache()]=tmp;
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
    
    if (checksum == ackPkt.checksum && in_wds(ackPkt.acknum)  ) {
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
        pns->sendToNetworkLayer(SENDER,pkt[i%N]);
    }
}
