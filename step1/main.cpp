#include "stdafx.h"
int main(int argc, char const *argv[])
{
    //�����Ҫʹ��������RdtЭ�飬ֻ��Ҫʵ������������Rdtʵ�����ʵ���� //��GBNRdtSender��GBNRdtSeceiver
    GBNRdtSender * ps = new GBNRdtSender();
    GBNRDTReceiver * pr = new GBNRDTReceiver();
    pns->init();
    pns->setRtdSender(ps);
    pns->setRtdReceiver(pr);
    pns->setInputFile("C:\\Users\\crackryan\\Desktop\\input.txt"); 
    pns->setOutputFile("C:\\Users\\crackryan\\Desktop\\output.txt"); 
    pns->start();
    delete ps;
    delete pr;
    //ָ��Ψһ�Ĺ�����ʵ����ֻ��main��������ǰdelete
    delete pUtils; //ָ��Ψһ��ģ�����绷����ʵ����ֻ��main��������ǰdelete delete pns;
    return 0;
}