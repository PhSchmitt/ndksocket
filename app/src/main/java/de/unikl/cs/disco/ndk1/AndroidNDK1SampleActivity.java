package de.unikl.cs.disco.ndk1;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;


public class AndroidNDK1SampleActivity extends ActionBarActivity {

    static {
        System.loadLibrary("ndk1");
    }

    String hostname = "mptcpsrv1.philippschmitt.de";
    Integer port = 8080;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_android_ndk1_sample);
        System.loadLibrary("ndk1");
        final Button buttonUrgent = (Button) findViewById(R.id.buttonUrgent);
        final Button buttonUnimportant = (Button) findViewById(R.id.buttonUnimportant);

        try
        {
            int error = openConnection(hostname, port);
            if (0 == error) {
                Log.d("NDK: ","Connection opened");
            } else {
                Log.d("NDK: ","error opening connection" + error);
            }
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

        buttonUrgent.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                int errorcode = sendNative( "This is some sample " +
                            "data that should have set the Urgent flag \n", true);
                if (0 == errorcode) {
                    Log.d("NDK: ","Urgent data sent");
                } else {
                    Log.d("NDK: ","error sending urgent data" + errorcode);
                }
                buttonUrgent.setText("Urgent clicked");
            }
        });

        buttonUnimportant.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                int errorcode = sendNative( "This is some unimportant sample " +
                        "data that should not have set the Urgent flag \n", false);
                if (0 == errorcode) {
                    Log.d("NDK: ","Unimportant data sent");
                } else {
                    Log.d("NDK: ","error sending unimportant data" + errorcode);
                }
                buttonUnimportant.setText("Unimportant clicked");
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_android_ndk1_sample, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private native int openConnection(String jurl, int portno);

    private native int sendNative(String jdata, Boolean jSetUrgentFlag);

    private native int closeConnection();
}
