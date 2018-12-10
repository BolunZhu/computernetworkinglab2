#include "stdafx.h"
#include "GBNRdtReceiver.h"
GBNRDTReceiver::GBNRDTReceiver():base(0)
{
    last_ackpkt.acknum=-1;//if 1st pck lost then resend with acknum = -1
    last_ackpkt.checksum=0;
    last_ackpkt.seqnum=-1;//unuse
    for (int i = 0; i < Configuration::PAYLOAD_SIZE; i++){
		last_ackpkt.payload[i] = '-';
	}
    last_ackpkt.checksum=pUtils->calculateCheckSum(last_ackpkt);
}
void GBNRDTReceiver::receive(Packet &packet){
    
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