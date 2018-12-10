#include "stdafx.h"
#include "GBNRdtSender.h"

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
