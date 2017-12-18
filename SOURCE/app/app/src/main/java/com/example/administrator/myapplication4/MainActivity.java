package com.example.administrator.myapplication4;

import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
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
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    }
    private void restart(){
        SharedPreferences shared = getSharedPreferences("is", MODE_PRIVATE);
        SharedPreferences.Editor editor = shared.edit();
        editor.putBoolean("isfer", true);
        editor.commit();
    }
    public void jiami_onClick(View view){
        Intent intent = new Intent(MainActivity.this,jiami.class);
        startActivity(intent);
        overridePendingTransition( R.anim.abc_fade_in,R.anim.abc_fade_out);


    }
    public void jiemi_onClick(View view){
        Intent intent = new Intent(MainActivity.this,jiemi.class);
        startActivity(intent);
        overridePendingTransition( R.anim.abc_fade_in,R.anim.abc_fade_out);

    }
}
