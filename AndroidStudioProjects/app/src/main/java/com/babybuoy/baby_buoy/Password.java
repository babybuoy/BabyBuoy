package com.babybuoy.baby_buoy;

import android.content.Intent;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.FirebaseAuth;

public class Password extends AppCompatActivity {

    private EditText Email;
    private Button Reset;
    private FirebaseAuth firebaseAuth;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_password);

        Email = findViewById(R.id.passwordEmail);
        Reset = findViewById(R.id.passReset);

        firebaseAuth = FirebaseAuth.getInstance();

        Reset.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String useremail = Email.getText().toString().trim();

                if(useremail.equals("")){
                    Toast.makeText(Password.this, "Please enter your registered email", Toast.LENGTH_SHORT).show();
                }else{
                    firebaseAuth.sendPasswordResetEmail(useremail).addOnCompleteListener(new OnCompleteListener<Void>() {
                        @Override
                        public void onComplete(@NonNull Task<Void> task) {
                            if(task.isSuccessful()){
                                Toast.makeText(Password.this,"Password email reset sent!",Toast.LENGTH_SHORT).show();
                                finish();
                                startActivity(new Intent(Password.this,MainActivity.class));
                            }else{
                                Toast.makeText(Password.this,"Error: Email reset failed",Toast.LENGTH_SHORT).show();

                            }
                        }
                    });
                }
            }
        });

    }
}
