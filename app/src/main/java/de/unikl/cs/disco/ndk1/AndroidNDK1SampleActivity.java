package de.unikl.cs.disco.ndk1;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
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

        buttonUrgent.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
//                 Perform action on click
                try {
                    helloLog("This will log to LogCat via the native call.");
                    if (0 != sendUrgent(hostname, port, "This is some sample " +
                            "data that should have set the Urgent flag", true)) {
                        helloLog("Urgent data sent");
                    }
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                }

                buttonUrgent.setText("Urgent clicked");
            }
        });

        buttonUnimportant.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
//                 Perform action on click
                try {
                    helloLog("This will log to LogCat via the native call.");
                    if (0 != sendUrgent(hostname, port, "Just a few random text " +
                            "that is really unimportant and we don't care what happens - if you " +
                            "want, you can perform am MITM-attack on it and noone cares because " +
                            "this is just some random unimportant text that we use to see " +
                            "whether the flag is set or not and here it should not be set", false)) {
                        helloLog("Unimportant data sent");
                    }
                } catch (Exception e) {
                    e.printStackTrace();
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

    private native void helloLog(String logThis);

    private native int sendUrgent(String jurl, int portno, String jdata, boolean jSetUrgentFlag);
}
