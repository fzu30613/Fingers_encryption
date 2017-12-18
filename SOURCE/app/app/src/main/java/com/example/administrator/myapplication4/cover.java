package com.example.administrator.myapplication4;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class cover extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_cover);
        handler.postDelayed(runnable, 1000);
    }

    Handler handler = new Handler();
    Runnable runnable = new Runnable() {
        @Override
        public void run() {
            date();
            handler.postDelayed(this, 3000);
            handler.removeCallbacks(runnable);
        }
    };

    private void date() {
        SharedPreferences shared = getSharedPreferences("is", MODE_PRIVATE);
        boolean isfer = shared.getBoolean("isfer", true);
        SharedPreferences.Editor editor = shared.edit();
        if (isfer) {
            //第一次进入跳转
            Intent in = new Intent(cover.this, First_in.class);
            startActivity(in);
            finish();
            editor.putBoolean("isfer", false);
            editor.commit();
        } else {
            //第二次进入跳转
            Intent in = new Intent(cover.this, MainActivity.class);
            startActivity(in);
            finish();


        }
    }
    private void restart(){
        SharedPreferences shared = getSharedPreferences("is", MODE_PRIVATE);
        SharedPreferences.Editor editor = shared.edit();
        editor.putBoolean("isfer", true);
        editor.commit();
    }
}
