package com.example.hasee.net_test;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Binder;
import android.os.Build;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class NetManager extends Service {
    /*public static NetHandler nethandler;
    class NetHandler extends Handler{
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what){
                case 1:

                    break;
                default:
                    break;
            }
        }
    }*/
    /**/
    private boolean flag_UDPResponse;//UDPResponse运行标志
    /*UDP回应线程：接受UDP广播并回应设备信息（设备名，IP可从数据报中获取）*/
    class UDPResponse extends Thread{
        //private String LocalIP=getLocalIpAddress();//未实现
        private String PhoneName= Build.MODEL;//获取设备名称
        private String sendEnd="$";
        @Override
        public void run(){
            try {
                DatagramSocket sendSocket = new DatagramSocket(4999);

                while(flag_UDPResponse) {

                    /*接收UDP请求*/
                    byte[] receiveBuf_request = new byte[128];
                    DatagramPacket receivePacket = new DatagramPacket(receiveBuf_request,receiveBuf_request.length);
                    sendSocket.receive(receivePacket);
                    /*获得请求方ip*/
                    InetAddress address = receivePacket.getAddress();

                    /*发送*/
                    byte[] bytesToSend = (PhoneName+sendEnd).getBytes();
                    DatagramPacket sendPacket = new DatagramPacket(bytesToSend, bytesToSend.length,address , 5000);
                    sendSocket.send(sendPacket);

                }
            }catch(Exception e){
                e.printStackTrace();
            }
        }
    }
    public Thread udpR=new UDPResponse();//UDP回应线程实例

    private boolean flag_TCPManager;
    private boolean flag_TCPConnect;
    private boolean flag_TCPSend;
    private static final int TCP_PORT = 2228;
    private ServerSocket server = null;
    private Socket client = null;
    /*TCP服务器管理线程：管理每次TCP连接*/
    class TCPServer extends Thread{
        @Override
        public void run() {
            try{
                server= new ServerSocket(TCP_PORT);
                Log.i("NetManager","start server");
                while(flag_TCPManager){
                    flag_TCPConnect=true;
                    client=server.accept();
                    Log.i("NetManager","start accept");
                    InputStream inputstream=client.getInputStream();//创建输入数据流
                    while(flag_TCPConnect){
                        final byte[] buf = new byte[1024];
                        final int len = inputstream.read(buf);//获取输出流
                        String recv=new String(buf,0,len);
                        Log.i("NetManager","recv:"+recv);
                        if(recv.equals("encrypt")) {
                            Message msg =new Message();
                            msg.what=MainActivity.Encrypt;
                            Log.i("NetManager","msg.what:"+msg.what);
                            MainActivity.handler.sendMessage(msg);
                        }
                        else if(recv.equals("decrypt")) {
                            Message msg =new Message();
                            msg.what=MainActivity.Decrypt;
                            MainActivity.handler.sendMessage(msg);
                            Log.i("NetManager","msg.what:"+msg.what);
                        }
                        else if(recv.equals("close")){
                            flag_TCPConnect=false;
                            OutputStream outputStream = client.getOutputStream();//创建输出数据流
                            String data="close";
                            outputStream.write(data.getBytes());
                            break;
                        }
                        else continue;
                        flag_TCPSend=true;
                        Log.i("NetManager","flag_TCPSend:"+flag_TCPSend);
                        while(flag_TCPSend){
                            Thread.sleep(1000);
                        }
                    }
                    client.close();
                }
                server.close();
            }catch (Exception e){
                e.printStackTrace();
            }
        }
    }
    class TCPSend extends Thread{
        String data;
        TCPSend(String d){
            data=d;
        }
        @Override
        public void run() {
            Message msg0 =new Message();
            msg0.what=3;
            MainActivity.handler.sendMessage(msg0);
            try {
                if (flag_TCPSend) {
                    try{
                        client.sendUrgentData(0xFF);
                    }catch(Exception ex){
                        ex.printStackTrace();
                    }
                    OutputStream outputStream = client.getOutputStream();//创建输出数据流
                    outputStream.write(data.getBytes());
                }
                flag_TCPSend = false;
                Log.i("NetManager","flag_TCPSend:"+flag_TCPSend);
            }catch (IOException e){
                flag_TCPSend = false;
                flag_TCPConnect = false;
                e.printStackTrace();
            }
        }
    }
    class TCPManager extends Binder{
        public void send(String sendData){
            TCPSend send_thread=new TCPSend(sendData);
            send_thread.start();
        }
        public void cancel(){
            flag_TCPSend=false;
        }
    }

    public NetManager() {
        flag_UDPResponse=true;
        flag_TCPManager=true;
    }
    private TCPManager tcpM =new TCPManager();
    private Thread tcpS=new TCPServer();
    @Override
    public IBinder onBind(Intent intent) {
        return tcpM;
    }

    @Override
    public void onCreate() {
        udpR.start();
        tcpS.start();
        //nethandler = new NetHandler();
        super.onCreate();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        flag_UDPResponse=false;
        flag_TCPManager=false;
        super.onDestroy();
    }
}
