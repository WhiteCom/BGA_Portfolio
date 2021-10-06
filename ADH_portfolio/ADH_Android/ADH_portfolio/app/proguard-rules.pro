# Add project specific ProGuard rules here.
# You can control the set of applied configuration files using the
# proguardFiles setting in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class or.kr.busanit.helloc.MainActivity {
#   public *;
#}

-keep public class * extends android.app.Activity

-keepclassmembers class com.adh.game.App {
    com.adh.game.App app;
    public void sendMessage*(...);
}

#-keepclassmembers class com.adh.lib.Audio {
#   *;
#}

-keepclassmembers class com.adh.lib.Native {
   public native *(...);
}


# Uncomment this to preserve the line number information for
# debugging stack traces.
#-keepattributes SourceFile,LineNumberTable

# If you keep the line number information, uncomment this to
# hide the original source file name.
#-renamesourcefileattribute SourceFile