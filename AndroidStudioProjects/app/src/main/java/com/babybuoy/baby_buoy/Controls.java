package com.babybuoy.baby_buoy;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

public class Controls extends AppCompatActivity {

    private FirebaseAuth firebaseAuth;
    private Button logout, ProfileInfo;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_controls);

        firebaseAuth = FirebaseAuth.getInstance();

        logout = findViewById(R.id.btnLogout);
        ProfileInfo = findViewById(R.id.userSettings);

        ProfileInfo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(Controls.this, SettingsPage.class));
            }
        });

        logout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                firebaseAuth.signOut();
                finish();
                startActivity(new Intent(Controls.this, MainActivity.class));
            }
        });
    }
}
