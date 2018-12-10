#include "stdafx.h"
#include "SRRdtSender.h"
#include "Global.h"
SRRdtSender::SRRdtSender():base(0),nextnum(0){
    
    for(int i = 0; i < N; i++)
    {
        ok[i]=false;
    }
    
}
SRRdtSender::~SRRdtSender(){}

bool SRRdtSender::in_wds(int num)
{  // base<num<base+N || num<base+N<base || base+N<base<num return true 
    unsigned int up = (_base()+N)&(K_range-1);                                                 
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
bool SRRdtSender::getWaitingState(){
    return _nextnum()==_base()+N;
}
bool SRRdtSender::send(Message &message) {
    if(getWaitingState()){//full
        return false;
    }
    //make packet tmp
    Packet tmp;
    tmp.acknum=-1;//ignore this
    tmp.checksum=0;
    tmp.seqnum=_nextnum();
    memcpy(tmp.payload,message.data,sizeof(message.data));
    tmp.checksum=pUtils->calculateCheckSum(tmp);

    //copy tmp in cache pkt[N]
    pkt[_nextcache()]=tmp;
    //send pkt
    pns->sendToNetworkLayer(SENDER,pkt[_nextcache()]);
    pns->startTimer(SENDER,Configuration::TIME_OUT,_nextnum());
    nextnum++;
    return true;
}
void SRRdtSender::receive(Packet &ackPkt){
    int checksum = pUtils->calculateCheckSum(ackPkt);
    
    if (checksum == ackPkt.checksum && in_wds(ackPkt.acknum) ) 
    {
        //tag this pkt received
        ok[ackPkt.acknum]=true;
        //close his timer
        pns->stopTimer(SENDER,ackPkt.acknum);
        //slip foward until not receive
        while (ok[_base()]) {
            ok[_base()]=false;
            base++;
        }
    }
    else {
        // do nothing
    }
}
void SRRdtSender::timeoutHandler(int seqNum){
    //time out then restart timer
    pns->stopTimer(SENDER,seqNum);
    pns->startTimer(SENDER,Configuration::TIME_OUT,seqNum);
    //resend this
    pns->sendToNetworkLayer(SENDER,pkt[seqNum%N]);
}
