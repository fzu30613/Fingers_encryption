package com.example.hasee.net_test;

import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Handler;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import android.Manifest;

import android.content.pm.PackageManager;
import android.hardware.fingerprint.FingerprintManager;

import android.os.CancellationSignal;
import android.support.v4.app.ActivityCompat;

import android.util.Log;

import android.widget.Toast;
import android.os.Build;
import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.webkit.WebView;
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
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.net.NetworkInterface;
import java.util.Collections;
import java.util.List;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;
import android.content.ServiceConnection;

import android.os.IBinder;
import android.os.Message;
import android.support.v7.app.AlertDialog;


import android.widget.TextView;

import static android.provider.Settings.Secure.ANDROID_ID;


public class MainActivity extends AppCompatActivity{
    FingerprintManager manager;
    KeyguardManager mKeyManager;
    String b;
    private String c;
    private String d;
    private final static int REQUEST_CODE_CONFIRM_DEVICE_CREDENTIALS = 0;
    private final static String TAG = "finger_log";
    public static final int Encrypt=1;
    public static final int Decrypt=2;
    public static MyHandler handler;
    private int a = 0;
    private NetManager.TCPManager tcpM;
    private ServiceConnection connection=new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            tcpM=(NetManager.TCPManager)service;
        }
        @Override
        public void onServiceDisconnected(ComponentName name) {
        }
    };
    private TextView IP_Text;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        handler = new MyHandler();
        manager = (FingerprintManager) this.getSystemService(Context.FINGERPRINT_SERVICE);
        mKeyManager = (KeyguardManager) this.getSystemService(Context.KEYGUARD_SERVICE);


        setContentView(R.layout.activity_main);
        IP_Text=(TextView) findViewById(R.id.IP_Text);
        Intent start_bind_NetManager=new Intent(this,NetManager.class);
        startService(start_bind_NetManager);
        bindService(start_bind_NetManager,connection,BIND_AUTO_CREATE);
        //获取wifi服务
        WifiManager wifiManager = (WifiManager)getApplicationContext().getSystemService(WIFI_SERVICE);
        //判断wifi是否开启
        if (!wifiManager.isWifiEnabled()) {
            wifiManager.setWifiEnabled(true);
        }
        WifiInfo wifiInfo = wifiManager.getConnectionInfo();
        int ipAddress = wifiInfo.getIpAddress();
        String ip = intToIp(ipAddress);
        TextView et = (TextView) findViewById(R.id.IP_Text);
        et.setText(ip);
        TextView textmain= (TextView) findViewById(R.id.textman);;
        TextView abcd = (TextView) findViewById(R.id.abcd);
        b = Settings.System.getString(getContentResolver(), Settings.System.ANDROID_ID);
        //abcd.setText(Integer.toHexString(Integer.valueOf( b.substring(0,8),16)));
        textmain.setText("等待请求中");
        c=b.substring(0,8);
        d=b.substring(8,16);
    }
    private String intToIp(int i) {

        return (i & 0xFF ) + "." +
                ((i >> 8 ) & 0xFF) + "." +
                ((i >> 16 ) & 0xFF) + "." +
                ( i >> 24 & 0xFF) ;
    }
    @Override
    protected void onDestroy() {
        Intent stop_unbind_NetManager=new Intent(this,NetManager.class);
        unbindService(connection);
        stopService(stop_unbind_NetManager);
        super.onDestroy();
    }
    class MyHandler extends Handler{


        public void handleMessage(Message msg) {
            switch (msg.what){
                case Encrypt:

                    //tcpM.send("123");
                    Toast.makeText(MainActivity.this, "请识别指纹" , Toast.LENGTH_SHORT).show();
                    Log.i("MainActivity","Encrypt");
                    TextView textmain= (TextView) findViewById(R.id.textman);;
                    textmain.setText("接收到加密请求");
                    AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
                    builder.setMessage("加密\n请识别指纹");
                    builder.setPositiveButton("确定",new DialogInterface.OnClickListener(){
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            //TODO
                            a=0;
                            startListening(null);
                            TextView textmain= (TextView) findViewById(R.id.textman);;
                            textmain.setText("请识别指纹");

                        }
                    });
                    builder.show();
                    break;
                case Decrypt: a=1;



                    Log.i("MainActivity","Decrypt");
                    AlertDialog.Builder de_builder = new AlertDialog.Builder(MainActivity.this);
                    TextView textman= (TextView) findViewById(R.id.textman);;
                    textman.setText("接收到解密请求");
                    de_builder.setMessage("解密\n请识别指纹");
                    de_builder.setPositiveButton("确定",new DialogInterface.OnClickListener(){
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            //TODO
                            TextView textman= (TextView) findViewById(R.id.textman);;
                            textman.setText("请识别指纹");
                            startListening(null);
                            Toast.makeText(MainActivity.this, "请识别指纹" , Toast.LENGTH_SHORT).show();

                        }
                    });
                    de_builder.show();
                    break;
                case 3://test

                    break;
                default:
                    break;
            }
        }
    }
    public boolean isFinger() {

        //android studio 上，没有这个会报错
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.USE_FINGERPRINT) != PackageManager.PERMISSION_GRANTED) {
            Toast.makeText(this, "没有指纹识别权限", Toast.LENGTH_SHORT).show();
            return false;
        }

        //判断硬件是否支持指纹识别
        if (!manager.isHardwareDetected()) {
            Toast.makeText(this, "没有指纹识别模块", Toast.LENGTH_SHORT).show();
            return false;
        }

        //判断 是否开启锁屏密码

        if (!mKeyManager.isKeyguardSecure()) {
            Toast.makeText(this, "没有开启锁屏密码", Toast.LENGTH_SHORT).show();
            return false;
        }

        //判断是否有指纹录入
        if (!manager.hasEnrolledFingerprints()) {
            Toast.makeText(this, "没有录入指纹", Toast.LENGTH_SHORT).show();
            return false;
        }


        return true;
    }

    CancellationSignal mCancellationSignal = new CancellationSignal();
    //回调方法
    FingerprintManager.AuthenticationCallback mSelfCancelled = new FingerprintManager.AuthenticationCallback() {
        @Override
        public void onAuthenticationError(int errorCode, CharSequence errString) {
            //但多次指纹密码验证错误后，进入此方法；并且，不能短时间内调用指纹验证
            if(a==1){
                tcpM.send("$");
            }else{
                tcpM.send("@");
            }
            Toast.makeText(MainActivity.this, "多次失败，请稍后重试", Toast.LENGTH_SHORT).show();
            TextView textmain= (TextView) findViewById(R.id.textman);;
            textmain.setText("多次失败，请稍后重试");
            handler.postDelayed(runnable, 2000);
            showAuthenticationScreen();
        }

        @Override
        public void onAuthenticationHelp(int helpCode, CharSequence helpString) {

            Toast.makeText(MainActivity.this, helpString, Toast.LENGTH_SHORT).show();
        }

        @Override
        public void onAuthenticationSucceeded(FingerprintManager.AuthenticationResult result) {

            Toast.makeText(MainActivity.this, "指纹识别成功", Toast.LENGTH_SHORT).show();
            //tcpM.send("aa");
            if(a==0) {
               // tcpM.send("asd");
                tcpM.send(c+"@"+d);
            }else{
                tcpM.send(c);
            }TextView textmain= (TextView) findViewById(R.id.textman);;
            textmain.setText("指纹识别成功");
            handler.postDelayed(runnable, 2000);
        }
        Handler handler = new Handler();
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                TextView textmain= (TextView) findViewById(R.id.textman);;
                textmain.setText("等待请求中");
                handler.removeCallbacks(runnable);
            }
        };

        @Override
        public void onAuthenticationFailed() {
            Toast.makeText(MainActivity.this, "指纹识别失败", Toast.LENGTH_SHORT).show();

        }
    };


    public void startListening(FingerprintManager.CryptoObject cryptoObject) {
        //android studio 上，没有这个会报错
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.USE_FINGERPRINT) != PackageManager.PERMISSION_GRANTED) {
            Toast.makeText(MainActivity.this, "没有指纹识别权限", Toast.LENGTH_SHORT).show();
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

}
