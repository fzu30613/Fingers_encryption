package com.example.administrator.myapplication4;

import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.Manifest;
import android.app.KeyguardManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.hardware.fingerprint.FingerprintManager;
import android.os.Bundle;
import android.os.CancellationSignal;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;

public class jiemi extends AppCompatActivity {
    FingerprintManager manager;
    KeyguardManager mKeyManager;
    private final static int REQUEST_CODE_CONFIRM_DEVICE_CREDENTIALS = 0;
    private final static String TAG = "finger_log";
    boolean isConnect=true;//连接还是断开
    boolean issend=false;
    EditText IPEditText;//定义ip输入框
    EditText MsgEditText;//定义信息输出框
    Socket socket = null;//定义socket
    Button jiemibutton;
    Button jiemireturn;
    private OutputStream outputStream=null;//定义输出流
    private InputStream inputStream=null;//定义输入流
    int a=0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_jiemi);
        IPEditText = (EditText) findViewById(R.id.edittext_jiemi);
        jiemibutton = (Button) findViewById(R.id.jiemibutton);
        jiemireturn = (Button) findViewById(R.id.jiemireturn);
        manager = (FingerprintManager) this.getSystemService(Context.FINGERPRINT_SERVICE);
        mKeyManager = (KeyguardManager) this.getSystemService(Context.KEYGUARD_SERVICE);
    }
    public void jiemizhiwenyanzheng_onclick(View view){
        Toast.makeText(jiemi.this, "请进行指纹识别", Toast.LENGTH_LONG).show();
        Log(TAG, "keyi");
        startListening(null);
    }
public void jiemireturn_onclick(View view){
        finish();
}
    public boolean isFinger() {

        //android studio 上，没有这个会报错
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.USE_FINGERPRINT) != PackageManager.PERMISSION_GRANTED) {
            Toast.makeText(this, "没有指纹识别权限", Toast.LENGTH_SHORT).show();
            return false;
        }
        Log(TAG, "有指纹权限");
        //判断硬件是否支持指纹识别
        if (!manager.isHardwareDetected()) {
            Toast.makeText(this, "没有指纹识别模块", Toast.LENGTH_SHORT).show();
            return false;
        }
        Log(TAG, "有指纹模块");
        //判断 是否开启锁屏密码

        if (!mKeyManager.isKeyguardSecure()) {
            Toast.makeText(this, "没有开启锁屏密码", Toast.LENGTH_SHORT).show();
            return false;
        }
        Log(TAG, "已开启锁屏密码");
        //判断是否有指纹录入
        if (!manager.hasEnrolledFingerprints()) {
            Toast.makeText(this, "没有录入指纹", Toast.LENGTH_SHORT).show();
            return false;
        }
        Log(TAG, "已录入指纹");

        return true;
    }

    CancellationSignal mCancellationSignal = new CancellationSignal();
    //回调方法
    FingerprintManager.AuthenticationCallback mSelfCancelled = new FingerprintManager.AuthenticationCallback() {
        @Override
        public void onAuthenticationError(int errorCode, CharSequence errString) {
            //但多次指纹密码验证错误后，进入此方法；并且，不能短时间内调用指纹验证
            Toast.makeText(jiemi.this, errString, Toast.LENGTH_SHORT).show();
            showAuthenticationScreen();
        }

        @Override
        public void onAuthenticationHelp(int helpCode, CharSequence helpString) {

            Toast.makeText(jiemi.this, helpString, Toast.LENGTH_SHORT).show();
        }

        @Override
        public void onAuthenticationSucceeded(FingerprintManager.AuthenticationResult result) {
            jiemibutton.setEnabled(false);
            jiemireturn.setEnabled(false);
            jiemibutton.setBackground(getDrawable(R.drawable.sending));
            Toast.makeText(jiemi.this, "指纹识别成功", Toast.LENGTH_SHORT).show();
            handle.postDelayed(runnabl, 1000);



            handler.postDelayed(runnable, 2100);


        }
        Handler handle = new Handler();
        Runnable runnabl = new Runnable() {
            @Override
            public void run() {
                Connect();
                Send();
                handle.removeCallbacks(runnabl);
            }
        };
        Handler handler = new Handler();
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                Connect();
                Toast.makeText(jiemi.this, "传输完成", Toast.LENGTH_SHORT).show();
                handl.postDelayed(runnab, 2000);
                jiemireturn.setEnabled(true);
                jiemibutton.setBackground(getDrawable(R.drawable.sendover));
                handler.removeCallbacks(runnable);
            }
        };
        Handler handl = new Handler();
        Runnable runnab = new Runnable() {
            @Override
            public void run() {
                jiemibutton.setEnabled(true);
                jiemibutton.setBackground(getDrawable(R.drawable.send));
                handl.removeCallbacks(runnab);
            }
        };
        @Override
        public void onAuthenticationFailed() {
            Toast.makeText(jiemi.this, "指纹识别失败", Toast.LENGTH_SHORT).show();
            a++;
            if(a==5){
                Connect();
                Send1();
                Toast.makeText(jiemi.this, "认证失败，请稍后再试", Toast.LENGTH_SHORT).show();
                handler.postDelayed(runnable, 1000);
            }
        }
    };


    public void startListening(FingerprintManager.CryptoObject cryptoObject) {
        //android studio 上，没有这个会报错
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.USE_FINGERPRINT) != PackageManager.PERMISSION_GRANTED) {
            Toast.makeText(this, "没有指纹识别权限", Toast.LENGTH_SHORT).show();
            return;
        }
        manager.authenticate(cryptoObject, mCancellationSignal, 0, mSelfCancelled, null);


    }

    /**
     * 锁屏密码
     */
    private void showAuthenticationScreen() {

        Intent intent = mKeyManager.createConfirmDeviceCredentialIntent("finger", "测试指纹识别");
        if (intent != null) {
            startActivityForResult(intent, REQUEST_CODE_CONFIRM_DEVICE_CREDENTIALS);
        }
    }



    private void Log(String tag, String msg) {
        Log.d(tag, msg);
    }
    public void Connect() {
        if (isConnect == true) //标志位 = true表示连接
        {
            //启动连接线程
            jiemi.Connect_Thread connect_Thread = new jiemi.Connect_Thread();
            connect_Thread.start();
            try{
                Thread.sleep(1000);
            }catch(Exception e){
                e.printStackTrace();//退出程序
            }
            if(socket!=null){
                isConnect = false;//置为false
                Toast.makeText(jiemi.this, "连接成功", Toast.LENGTH_SHORT).show();
            }
        }
        else //标志位 = false表示退出连接
        {

            isConnect = true;//置为true
            try
            {
                socket.close();//关闭连接
                socket=null;
                Toast.makeText(jiemi.this,"传输完成",Toast.LENGTH_SHORT).show();
            }
            catch (IOException e)
            {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
    }
    public void Send1()
    {
        jiemi.Send_Thread1 send_Thread = new jiemi.Send_Thread1();
        send_Thread.start();
    }
    class Send_Thread1 extends Thread//回传错误
    {
        public void run()//重写run方法
        {
            try
            {
                if(socket!=null){
                    //获取输出流
                    outputStream = socket.getOutputStream();
                    //发送数据
                    outputStream.write("no".getBytes());
                    //outputStream.write("0".getBytes());
                    Toast.makeText(jiemi.this,"已发送",Toast.LENGTH_SHORT).show();
                    Thread.sleep(1000);
                    issend=true;
                }
                else
                {
                    isConnect = true;
                    Toast.makeText(jiemi.this,"发送失败，请重连",Toast.LENGTH_SHORT).show();
                }
            }
            catch (Exception e)
            {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
    }
    public void Send()
    {
        jiemi.Send_Thread send_Thread = new jiemi.Send_Thread();
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
                    Toast.makeText(jiemi.this,"已发送",Toast.LENGTH_SHORT).show();

                    Thread.sleep(1000);

                    issend=true;
                }
                else
                {
                    isConnect = true;
                    Toast.makeText(jiemi.this,"发送失败，请重连",Toast.LENGTH_SHORT).show();
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
                    jiemi.Receive_Thread receive_Thread = new jiemi.Receive_Thread();
                    receive_Thread.start();
                    Toast.makeText(jiemi.this,"OK",Toast.LENGTH_SHORT).show();
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
