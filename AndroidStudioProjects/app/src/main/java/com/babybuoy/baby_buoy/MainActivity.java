package com.babybuoy.baby_buoy;

import android.app.ProgressDialog;
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
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;



public class MainActivity extends AppCompatActivity {

    private EditText Name, Password;
    public Button Login, Registrations, Reset;
    private FirebaseAuth firebaseAuth;
    private ProgressDialog progressDialog;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        progressDialog = new ProgressDialog(this);

        firebaseAuth = FirebaseAuth.getInstance();
        FirebaseUser user = firebaseAuth.getCurrentUser();

        Name = findViewById(R.id.mainUsername);
        Password = findViewById(R.id.mainPassword);
        Login = findViewById(R.id.btnLogin);
        Registrations = findViewById(R.id.btnRegister);
        Reset = findViewById(R.id.ForgotPassword);


        if (user != null) {
            finish();
            startActivity(new Intent(MainActivity.this, Controls.class));

        }

        Reset.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(MainActivity.this, Password.class));
            }
        });

        Login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                validate(Name.getText().toString(), Password.getText().toString());
            }
        });

        Registrations.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(MainActivity.this, Registration.class));
            }
        });

    }

    private void validate(String userName, String userPassword) {

        progressDialog.setMessage("Loading...");
        progressDialog.show();

        firebaseAuth.signInWithEmailAndPassword(userName, userPassword).addOnCompleteListener(new OnCompleteListener<AuthResult>() {
            @Override
            public void onComplete(@NonNull Task<AuthResult> task) {
                if (task.isSuccessful()) {
                    progressDialog.dismiss();
                    checkEmailVerification();
                } else {
                    progressDialog.dismiss();
                    Toast.makeText(MainActivity.this, "Incorrect username/password", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    private void checkEmailVerification() {
       FirebaseUser firebaseUser = FirebaseAuth.getInstance().getCurrentUser();
        boolean emailflag = firebaseUser.isEmailVerified();

       if(emailflag){
            finish();
            startActivity(new Intent(MainActivity.this, Controls.class));
        } else {
            Toast.makeText(this, "Verify your email", Toast.LENGTH_SHORT).show();
            firebaseAuth.signOut();
        }

    }

}
