package com.adh.game;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.RelativeLayout;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import java.io.InputStream;
import java.io.OptionalDataException;
import java.lang.ref.WeakReference;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.adh.lib.Audio;
import com.adh.lib.Native;

/*
    1. 기존 프로젝트 복붙한다 폴더
    (game, lib), AndroidManifest.xml, Activity_main.xml

    2. 구글플레이 서비스적용
    - google id
    - leaderboard/achievement id
    - ad. id

    3. keystore
    -

    4. proguard *.pro 보호해야 할 클래스명, 함수명, 필드명

    1. file io
    2. png -> rgba
    3. texture
    4. shader
    5. font -> graphics -> rgba -> texture
 */

public class App extends AppCompatActivity
{
    public Object getAssetMgr()
    {
        return getAssets();
    }

    public static String appPrint(String str)
    {
        Paint paint = new Paint();
        paint.setTypeface(Typeface.DEFAULT);
        paint.setTextScaleX(1.0f);

        Typeface typeface; //setStringName;
        typeface = Typeface.SERIF;
        typeface = Typeface.createFromAsset(app.getAssets(), "assets/xxx.otf");

        paint.setTypeface(typeface);

        //paint.set
        String sss = "Hello World";
        float width = paint.measureText(sss);

        Rect rect = new Rect();
        paint.getTextBounds(sss, 0, sss.length(), rect);
        float x = rect.left;
        float y = rect.top;
        float w = rect.right - x;
        float h = rect.bottom - y;

        //Log.e("ADH", "!!!!!!!!!appPrint!!!!!!!!!!"+str);

        Activity activity = new Activity();
        activity.finish();

        return "str";
    }
    public void appPrint2()
    {
        //Log.e("ADH", "!!!!!!!!!appPrint222222222!!!!!!!!!!");
    }

    public static App app;
    int field;

    int getField(){return field;}
    void setField(int f){field=f;}

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        app = this;

        RelativeLayout layout = new RelativeLayout(getApplicationContext());
        RelativeLayout.LayoutParams parms = new RelativeLayout.LayoutParams(
                RelativeLayout.LayoutParams.MATCH_PARENT,
                RelativeLayout.LayoutParams.MATCH_PARENT);
        //parms.addRule();
        layout.setLayoutParams(parms);

        GLView view = new GLView(getApplicationContext());
        layout.addView(view);

        setContentView(layout);
        //if( true )
        //    return;

        //if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
        //    getWindow().setFlags(//FLAG_LAYOUT_NO_LIMITS, FLAG_FULLSCREEN
        //            WindowManager.LayoutParams.FIRST_SYSTEM_WINDOW,
        //            WindowManager.LayoutParams.FIRST_SYSTEM_WINDOW);
        //}

        getWindow().setFlags(
                WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        view.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY |
                View.SYSTEM_UI_FLAG_FULLSCREEN |
                View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

        //#issue! Audio path
        //Audio audio = new Audio();
        //audio.run();
    }

    MyHandler myHandler = new MyHandler(this);
    public void sendMessage(int what, String data)
    {
        Message msg = new Message();
        msg.what = what;
        msg.obj = new String(data);
        myHandler.sendMessage(msg);
    }

    public void eventMessage(Message msg)
    {
        //Log.i("csm", "what = "+msg.what);
        if( msg.what==Msg_Login )
        {
            String strID = (String)msg.obj;
        }
        else if (msg.what==Msg_Logout)
        {
        }
    }

    public final static int Msg_Login = 0;
    public final static int Msg_Logout = 1;
    public final static int Msg_Cloud_Write = 2;
    public final static int Msg_Cloud_Read = 3;

    static class MyHandler extends Handler
    {
        WeakReference<App> handler;

        MyHandler(App app)
        {
            handler = new WeakReference<App>(app);
        }

        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);

            App a = handler.get();
            if( a!=null )
                a.eventMessage(msg);
        }
    }

    @Override
    protected void onStart()
    {
        super.onStart();

        // do something
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();

        Native.freeGame();
    }

    @Override
    protected void onStop()
    {
        super.onStop();

        // do something
    }

    @Override
    protected void onPause()
    {
        super.onPause();

        Native.pauseGame(true);
    }

    @Override
    protected void onResume()
    {
        super.onResume();

        Native.pauseGame(false);
    }
}

class GLView extends GLSurfaceView
{
    public GLView(Context context)
    {
        super(context);

        setEGLContextClientVersion(2);
        setEGLConfigChooser(8, 8, 8, 8, 8, 0);
        getHolder().setFormat(PixelFormat.RGBA_8888);
        setRenderer(new Render(context));
        //setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        //setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

        enableMulti = false;
        mi = new int[20];
        mx = new float[20];
        my = new float[20];
    }

    boolean enableMulti;
    int mi[];
    float mx[];
    float my[];

    public void setEnableMulti(boolean enable)
    {
        enableMulti = enable;
    }

    @Override
    public boolean onTouchEvent(MotionEvent e)
    {
        super.onTouchEvent(e);

        //Log.e("ADH", "onTouchEvent");
        // one
        int state;
        switch(e.getAction()) {
            case MotionEvent.ACTION_DOWN:   state = 0; break;
            case MotionEvent.ACTION_MOVE:   state = 1; break;
            case MotionEvent.ACTION_UP:     state = 2; break;
            default:                        state = e.getAction(); break;
        }
        Native.keyGame(state, e.getX(), e.getY());

        if( !enableMulti )
            return true;

        // multi
        int i, j, num = e.getPointerCount(), upIndex = 0xFF;

        switch( e.getAction() & MotionEvent.ACTION_MASK ) {
            case MotionEvent.ACTION_CANCEL:
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
                upIndex = (e.getAction() & MotionEvent.ACTION_POINTER_ID_MASK) >> MotionEvent.ACTION_POINTER_ID_SHIFT;
                break;
        }

        for(i=0, j=0; i<num; i++)
        {
            if( i==upIndex) continue;

            mi[j] = e.getPointerId(i);
            mx[j] = e.getX(i);
            my[j] = e.getY(i);
            j++;
        }
        Native.multiKeyGame(state, j, mx, my);
        return true;
    }
}

class Render implements GLSurfaceView.Renderer
{
    Context context;
    public Render(Context context) { this.context = context; }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig)
    {
        String apkFilePath = null;
        ApplicationInfo appInfo = null;
        PackageManager packageManager = context.getPackageManager();
        try {
            String packageName = App.app.getApplication().getPackageName();
            appInfo = packageManager.getApplicationInfo(packageName, 0);
        } catch(Exception e)
        {
            e.printStackTrace();
            throw new RuntimeException("경로 찾다가, 프로그램 종료되었습니다.");
        }
        apkFilePath = appInfo.sourceDir;

        String ioPath = context.getFilesDir().getAbsolutePath();

        Native.loadGame(apkFilePath, ioPath);
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int w, int h) { Native.resizeGame(w, h); }
    @Override
    public void onDrawFrame(GL10 gl10) { Native.drawGame(); }
}


