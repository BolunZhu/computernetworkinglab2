#include "stdafx.h"
#include "SRRdtReceiver.h"

SRRDTReceiver::SRRDTReceiver():base(0)
{
    last_ackpkt.acknum=-1;//if 1st pck lost then resend with acknum = -1
    last_ackpkt.checksum=0;
    last_ackpkt.seqnum=-1;//unuse
    for (int i = 0; i < Configuration::PAYLOAD_SIZE; i++){
		last_ackpkt.payload[i] = '-';
	}
    last_ackpkt.checksum=pUtils->calculateCheckSum(last_ackpkt);
}
void SRRDTReceiver::receive(Packet &packet){
    
    if (packet.checksum == pUtils->calculateCheckSum(packet) && packet.seqnum==base ) {
        // not_corrupt && is_expected_ACK
        // give it to up-layer
        Message msg;
        memcpy(msg.data,packet.payload,sizeof(packet.payload));
        pns->delivertoAppLayer(RECEIVER,msg);
        // update last_ackpkt
        last_ackpkt.acknum=packet.seqnum;
        last_ackpkt.checksum=pUtils->calculateCheckSum(last_ackpkt);
        pns->sendToNetworkLayer(SENDER,last_ackpkt);
    }
    else {
        //if not correct then resend last ackpkt
        pns->sendToNetworkLayer(RECEIVER,last_ackpkt);
    }
    
}
void send_ackpkt(int seqnum){
    Packet pkt;
    pkt.acknum=seqnum;
    pkt.checksum=0;
    pkt.seqnum=-1;//ignore
    for (int i = 0; i < Configuration::PAYLOAD_SIZE; i++){
		pkt.payload[i] = '-';
	}
    pkt.checksum=pUtils->calculateCheckSum(pkt);
    pns->sendToNetworkLayer(RECEIVER,pkt);
}
void SRRDTReceiver::receive(Packet &packet){
    if(ok && in_wds )
    {
        // tag it & store it
        // make_ackpkt & send ; 
        send_ackpkt(seqnum);
        // while(base is ok)
        // { 
                // base not ok
                // give it to up-layer
                // base++
        //  }
    }
    else
    {
        // send last ackpkt; 
        send_ackpkt(_base());
    }
}