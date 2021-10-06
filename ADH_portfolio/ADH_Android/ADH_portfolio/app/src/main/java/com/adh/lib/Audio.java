package com.adh.lib;

import android.content.Context;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.util.Log;

import com.adh.game.App;

import java.io.IOException;
import java.io.InputStream;


public class Audio implements Runnable
{
    AudioTrack audioTrack;
    boolean isPlaying;

    byte[] bgm;
    int offBgm;

    byte[] eff;
    int offEff;

    boolean toggle;

    int bufferSize;

    public Audio() {
        int sampleRates = 22050; //44100;
        int channelCount = AudioFormat.CHANNEL_IN_STEREO;
        int audioFormat = AudioFormat.ENCODING_PCM_16BIT;
        bufferSize = AudioTrack.getMinBufferSize(sampleRates, channelCount, audioFormat);

        audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC,
                                    sampleRates,
                                    channelCount,
                                    audioFormat,
                                    bufferSize,
                                    AudioTrack.MODE_STREAM
                                    );

        isPlaying = true;

        bgm = getStream("01 Over the Horizon.wav");
        offBgm = 0;
        eff = getStream("btn.wav");
        offEff = 0;

        offBgm = 0;

        toggle = false;

        new Thread(this).start();
    }

    byte[] getStream(String fileName)
    {
        byte[] b = null;

        try {
            InputStream is = App.app.getAssets().open(fileName);
            int len = is.available();
            b = new byte[len];
            is.read(bgm);
            is.close();
            return b;
        } catch (Exception e) {
            Log.e("ADH", e.toString());
            return null;
        }
    }

    public void run()
    {
        audioTrack.play(); //오디오를 설정하는 것이라고 생각

        while(isPlaying) //실제로 오디오가 나오는 구간
        {
            try {
                if(toggle)
                {
                    int len = audioTrack.write(bgm, offBgm, bufferSize);
                    offBgm += len;
                    if(offBgm >= bgm.length)
                        offBgm = 0; //배경음 무한반복
                }
                else {
                    int len = audioTrack.write(eff, offEff, bufferSize);
                    offEff += len;
                    if(offEff >= eff.length)
                        offEff = 0; //배경음 무한반복
                }
                toggle = !toggle;
            } catch (Exception e) {}
        }

        audioTrack.stop();
        audioTrack.release();
    }

}
