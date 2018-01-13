package com.example.hasee.net_test;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

public class First_in extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_first_in);






    }
    private void restart(){
        SharedPreferences shared = getSharedPreferences("is", MODE_PRIVATE);
        SharedPreferences.Editor editor = shared.edit();
        editor.putBoolean("isfer", true);
        editor.commit();
    }
    public void acceptclick(View v) {
        Intent intent = new Intent(First_in.this,rescue.class);
        startActivity(intent);
        finish();
    }
    public void noacceptonClick(View v) {
        restart();
        finish();
    }
}
