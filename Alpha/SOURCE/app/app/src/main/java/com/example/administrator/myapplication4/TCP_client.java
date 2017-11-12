package com.example.administrator.myapplication4;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.lang.*;
import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class TCP_client extends Activity {

    boolean isConnect=true;//连接还是断开
    Button ConnectButton;//定义连接按钮
    Button SendButton;//定义发送按钮
    EditText IPEditText;//定义ip输入框
    EditText PortText;//定义端口输入框
    EditText MsgEditText;//定义信息输出框
    EditText RrceiveEditText;//定义信息输入框
    Socket socket = null;//定义socket
    private OutputStream outputStream=null;//定义输出流
    private InputStream inputStream=null;//定义输入流


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout);

        ConnectButton = (Button) findViewById(R.id.Connect_Bt);//获得连接按钮对象
        SendButton = (Button) findViewById(R.id.Send_Bt);//获得发送按钮对象
        IPEditText = (EditText) findViewById(R.id.ip_ET);//获得ip文本框对象
        RrceiveEditText = (EditText) findViewById(R.id.Receive_ET);//获得接收消息文本框对象
    }

    public void Connect() {
        if (isConnect == true) //标志位 = true表示连接
        {
            //启动连接线程
            Connect_Thread connect_Thread = new Connect_Thread();
            connect_Thread.start();
            try{
                Thread.sleep(100);
            }catch(Exception e){
                e.printStackTrace();//退出程序
            }
            if(socket!=null){
                isConnect = false;//置为false
                Toast.makeText(TCP_client.this, "连接成功", Toast.LENGTH_SHORT).show();
            }
        }
        else //标志位 = false表示退出连接
        {
            isConnect = true;//置为true
            try
            {
                socket.close();//关闭连接
                socket=null;
                Toast.makeText(TCP_client.this,"连接断开",Toast.LENGTH_SHORT).show();
            }
            catch (IOException e)
            {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
    }

    public void Send()
    {
        Send_Thread  send_Thread = new Send_Thread();
        send_Thread.start();
    }
    class Send_Thread extends Thread//继承Thread
    {
        public void run()//重写run方法
        {
            try
            {
                if(socket!=null){
                    //获取输出流
                    outputStream = socket.getOutputStream();
                    //发送数据
                    outputStream.write("yes".getBytes());
                    //outputStream.write("0".getBytes());
                    Toast.makeText(TCP_client.this,"已发送",Toast.LENGTH_SHORT).show();
                    Thread.sleep(100);
                    outputStream.write("yes".getBytes());
                    //outputStream.write("0".getBytes());
                    Toast.makeText(TCP_client.this,"已发送",Toast.LENGTH_SHORT).show();
                }
                else
                {
                    isConnect = true;
                    ConnectButton.setText("连接");
                    Toast.makeText(TCP_client.this,"发送失败，请重连",Toast.LENGTH_SHORT).show();
                }
            }
            catch (Exception e)
            {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
    }
    //连接线程
    class Connect_Thread extends Thread//继承Thread
    {
        public void run()//重写run方法
        {
            try
            {
                if (socket == null)
                {
                    //用InetAddress方法获取ip地址
                    InetAddress ipAddress = InetAddress.getByName(IPEditText.getText().toString());
                    int port =Integer.valueOf("2228");//获取端口号
                    socket = new Socket(ipAddress, port);//创建连接地址和端口-------------------这样就好多了
                    //在创建完连接后启动接收线程
                    Receive_Thread receive_Thread = new Receive_Thread();
                    receive_Thread.start();
                    Toast.makeText(TCP_client.this,"OK",Toast.LENGTH_SHORT).show();
                }
            }
            catch (Exception e)
            {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
    }
    //接收线程
    class Receive_Thread extends Thread
    {
        public void run()//重写run方法
        {
            try
            {
                while (true)
                {
                    final byte[] buffer = new byte[1024];//创建接收缓冲区
                    if(socket ==null)break;
                    inputStream = socket.getInputStream();
                    final int len = inputStream.read(buffer);//数据读出来，并且返回数据的长度
                    //String msg0=new String(buffer,0,len);
                    //if(!msg0.equals("%close")) {
                    runOnUiThread(new Runnable()//不允许其他线程直接操作组件，用提供的此方法可以
                    {
                        public void run() {
                            // TODO Auto-generated method stub
                            //RrceiveEditText.setText(new String(buffer, 0, len));
                        }
                    });
                    /*}
                    else
                    {
                        isConnect = true;
                        ConnectButton.setText("连接");
                        Toast.makeText(TCP_client.this,"断开",Toast.LENGTH_SHORT).show();
                        /*try{
                            socket.close();
                            Thread.sleep(100);
                        }catch(Exception e){
                            e.printStackTrace();//退出程序
                        }
                        if(socket!=null) socket.close();
                        break;
                    }*/
                }
            }
            catch (Exception e)
            {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
    }

}