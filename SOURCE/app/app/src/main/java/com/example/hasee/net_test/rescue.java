package com.example.hasee.net_test;

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

public class rescue extends AppCompatActivity {
    int broj=5;   //记录时间

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_rescue);
        Button button = (Button) findViewById(R.id.button_rescue);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(rescue.this,MainActivity.class);
                startActivity(intent);
                finish();
            }
        });
        TextView textview = (TextView) findViewById(R.id.textview_rescue);
        textview.setText(Integer.toHexString(Integer.valueOf( Settings.System.getString(getContentResolver(), Settings.System.ANDROID_ID).substring(8,16),16)));
        handler.postDelayed(runnable, 1000);
    }
    Handler handler = new Handler();
    Runnable runnable = new Runnable() {
        @Override
        public void run() {
            broj--;

            Button button = (Button) findViewById(R.id.button_rescue);
            String sting;
            sting = String.valueOf(broj);
            button.setText(sting+"s");
            handler.postDelayed(this, 1000);
            if(broj==0){
                handler.removeCallbacks(runnable);
                button.setText("我已经记录救援代码了");
                button.setEnabled(true);
            }
        }
    };
    public String getTime() {
        long time = System.currentTimeMillis() / 1000;//获取系统时间的10位的时间戳
        String str = String.valueOf(time);
        return str;
    }
    private String getNewMac() {
        try {
            List<NetworkInterface> all = Collections.list(NetworkInterface.getNetworkInterfaces());
            for (NetworkInterface nif : all) {
                if (!nif.getName().equalsIgnoreCase("wlan0")) continue;

                byte[] macBytes = nif.getHardwareAddress();
                if (macBytes == null) {
                    return null;
                }

                StringBuilder res1 = new StringBuilder();
                for (byte b : macBytes) {
                    res1.append(String.format("%02X:", b));
                }

                if (res1.length() > 0) {
                    res1.deleteCharAt(res1.length() - 1);
                }
                return res1.toString();
            }
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        return null;
    }

}
