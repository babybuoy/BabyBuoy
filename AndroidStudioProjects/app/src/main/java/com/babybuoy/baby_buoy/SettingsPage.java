package com.babybuoy.baby_buoy;

import android.content.Intent;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class SettingsPage extends AppCompatActivity {

    private Button Edit,changePass;
    private TextView profileName, profileEmail;
    public FirebaseAuth firebaseAuth;
    public FirebaseDatabase firebaseDatabase;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings_page);

        Edit = findViewById(R.id.btnUpdate);
        profileName = findViewById(R.id.profilename);
        profileEmail = findViewById(R.id.profileemail);
        changePass = findViewById(R.id.btnchangePass);

        firebaseDatabase = FirebaseDatabase.getInstance();
        firebaseAuth = FirebaseAuth.getInstance();

        DatabaseReference databaseReference = firebaseDatabase.getReference(firebaseAuth.getUid());

        databaseReference.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                 UserProfile userProfile = dataSnapshot.getValue(UserProfile.class);
                 profileName.setText("Name:   " + userProfile.getUserName());
                 profileEmail.setText("Email:   " + userProfile.getUserEmail());
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Toast.makeText(SettingsPage.this,databaseError.getCode() , Toast.LENGTH_SHORT).show();
            }
        });

        Edit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(SettingsPage.this,UpdateProfile.class ));
            }
        });

        changePass.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(SettingsPage.this,UpdatePassword.class ));

            }
        });

    }
}
