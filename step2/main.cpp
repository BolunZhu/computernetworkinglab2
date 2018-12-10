#include "stdafx.h"
int main(int argc, char const *argv[])
{
    //如果需要使用其它的Rdt协议，只需要实例化其他具体Rdt实现类的实例， //如GBNRdtSender和GBNRdtSeceiver
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
    //指向唯一的工具类实例，只在main函数结束前delete
    delete pUtils; //指向唯一的模拟网络环境类实例，只在main函数结束前delete delete pns;
    return 0;
}