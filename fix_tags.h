#pragma once
namespace FIX {
    constexpr char SOH = '\x01';
    namespace Tags {
        constexpr int BeginString = 8;
        constexpr int BodyLength = 9;
        constexpr int MsgType = 35;
        constexpr int CheckSum = 10;
        constexpr int SenderCompID = 49;
        constexpr int TargetCompID = 56;
        constexpr int MsgSeqNum = 34;
        constexpr int SendingTime = 52;
        constexpr int ClOrdID = 11;
        constexpr int Symbol = 55;
        constexpr int Side = 54;
        constexpr int OrderQty = 38;
        constexpr int Price = 44;
        constexpr int OrdType = 40;
        constexpr int TimeInForce = 59;
        constexpr int OrdStatus = 39;
        constexpr int ExecType = 150;
        constexpr int OrderID = 37;
        constexpr int ExecID = 17;
        constexpr int EncryptMethod = 98;
        constexpr int HeartBtInt = 108;
        constexpr int TransactTime = 60;
    }
    namespace MsgTypes {
        constexpr char Logon[] = "A";
        constexpr char Logout[] = "5";
        constexpr char NewOrderSingle[] = "D";
        constexpr char ExecutionReport[] = "8";
    }
}
