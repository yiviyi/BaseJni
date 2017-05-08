package com.eve.basejni;

/**
 * Created by eve
 * yiviyi@github.com
 * on 2017/5/8.
 */

public class ClassField {
    private static int num;

    private String str;



    public static int getNum() {
        return num;
    }

    public static void setNum(int num) {
        ClassField.num = num;
    }

    public String getStr() {
        return str;
    }

    public void setStr(String str) {
        this.str = str;
    }
}
