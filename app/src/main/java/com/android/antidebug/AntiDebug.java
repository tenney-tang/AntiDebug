package com.android.antidebug;

import android.os.Process;

public class AntiDebug {
    private static String KEY_SUBSTRATE = "com.saurik.substrate.MS$2";

    static {
        System.loadLibrary("AntiDebug");
    }
    public static native void checkDebug();

    /* 通过堆栈判断apk是否被注入，被注入时ZygoteInit对象会被执行两次 */
    public static boolean isInject() {
        try {
            throw new Exception("");
        } catch (Exception e) {
            int i = 0;
            for (StackTraceElement stackTraceElement : e.getStackTrace()) {
                if (stackTraceElement.getClassName().equals("com.android.internal.os.ZygoteInit")) {
                    i++;
                    if (i == 2) {
                        Process.killProcess(Process.myPid());
                        return true;
                    }
                }
                if (stackTraceElement.getClassName().equals(KEY_SUBSTRATE)) {
                    Process.killProcess(Process.myPid());
                    return true;
                }
            }
            return false;
        }
    }
}
