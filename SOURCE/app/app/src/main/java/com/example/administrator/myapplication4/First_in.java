package com.example.administrator.myapplication4;

import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class First_in extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_first_in);
        Button accept = (Button) findViewById(R.id.button_accept);
        Button noaccept = (Button) findViewById(R.id.button_noaccept);
        accept.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(First_in.this,rescue.class);
                startActivity(intent);
                finish();
            }
        });
        noaccept.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                restart();
                finish();
            }
        });
    }
    private void restart(){
        SharedPreferences shared = getSharedPreferences("is", MODE_PRIVATE);
        SharedPreferences.Editor editor = shared.edit();
        editor.putBoolean("isfer", true);
        editor.commit();
    }
}
