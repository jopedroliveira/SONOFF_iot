package net.ddns.bnmhome.homesonoff;

import android.app.Activity;
import android.content.res.ColorStateList;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.loopj.android.http.*;

import org.json.JSONException;
import org.json.JSONObject;

import cz.msebera.android.httpclient.Header;

public class MainActivity extends AppCompatActivity {


    FloatingActionButton mainbutton;
    View mainView;
    //TextView text;
    static String status = "pending";
    static String ip = "****";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        getSupportActionBar().hide();


        mainView = findViewById(R.id.mainView);
        //text = (TextView) findViewById(R.id.command);
        mainbutton = (FloatingActionButton) findViewById(R.id.mainbutton);

        mainbutton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (status.equals("on")) {
                    invokeWS("LED=OFF");
                } else {
                    invokeWS("LED=ON");
                }
            }

        });

        invokeWS("status");

    }


    private void updateView() {
        //text.setText(status);
        if (status.equals("off")) {
            mainView.setBackgroundColor(ContextCompat.getColor(getApplicationContext(), R.color.invisible));
            mainbutton.setBackgroundTintList(new ColorStateList(new int[][]{
                            new int[]{0}}, new int[]{getResources().getColor(R.color.colorAccentDark)
                    })
            );

        } else if (status.equals("on")) {
            mainView.setBackground(getResources().getDrawable(R.drawable.circle));
            mainbutton.setBackgroundTintList(new ColorStateList(new int[][]{
                            new int[]{0}}, new int[]{getResources().getColor(R.color.colorAccent)
                    })
            );
        } else {
            //error
        }

    }

    private boolean invokeWS(String cmd) {
        AsyncHttpClient client = new AsyncHttpClient();
        client.get(getApplicationContext(), ip + cmd, new JsonHttpResponseHandler() {
            @Override
            public void onSuccess(int statusCode, Header[] headers, JSONObject responseBody) {
                try {
                    setResult(responseBody.getString("status"));
                    updateView();
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void onFailure(int statusCode, Header[] headers, Throwable throwable, JSONObject errorResponse) {
                setResult("ERROR");
                updateView();
            }
        });
        return true;
    }

    private void setResult(String st) {
        status = st;
    }
}
