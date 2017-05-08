package com.eve.basejni;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import butterknife.BindView;
import butterknife.ButterKnife;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {


    @BindView(R.id.btn_hello)
    Button btnHello;
    @BindView(R.id.tv_hello)
    TextView tvHello;
    @BindView(R.id.btn_Link)
    Button btnLink;
    @BindView(R.id.et_link1)
    EditText etLink1;
    @BindView(R.id.et_link2)
    EditText etLink2;
    @BindView(R.id.tv_Link)
    TextView tvLink;
    @BindView(R.id.btn_sum)
    Button btnSum;
    @BindView(R.id.et_sum_to)
    EditText etSumTo;
    @BindView(R.id.tv_sum)
    TextView tvSum;
    @BindView(R.id.btn_initArray)
    Button btnInitArray;
    @BindView(R.id.tv_initArray)
    TextView tvInitArray;
    private BaseUtils baseUtils;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
        initView();

        baseUtils = new BaseUtils();
        baseUtils.CallJavaMethod();
        baseUtils.CallJavaParamsMethod();

        BaseUtils.CallJavaInstanceMethod();
        BaseUtils.CallJavaStaticMethod();

        ClassField classField = new ClassField();
        classField.setNum(10);
        classField.setStr("hello");

        BaseUtils.UseInstanceField(classField);
        BaseUtils.UseStaticField();

        // 输出本地代码修改过后的值
        System.out.println("In Java--->ClassField.num = " + classField.getNum());
        System.out.println("In Java--->ClassField.str = " + classField.getStr());

        ButterKnife.bind(this);
    }

    private void initView() {
        btnHello.setOnClickListener(this);
        btnLink.setOnClickListener(this);
        btnSum.setOnClickListener(this);
        btnInitArray.setOnClickListener(this);
    }

    private void sumArr() {
        int len = (int) Math.floor(Double.valueOf(etSumTo.getText().toString().trim()));
        int[] arr = new int[len + 1];
        for (int i = 0; i < arr.length; i++) {
            arr[i] = i;
        }
        int sum = baseUtils.sumArray(arr);
        tvSum.setText("从1加到" + len + "总和是：" + sum);
    }

    private void initArray() {
        int[][] arr = baseUtils.initObjectArray(3);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                System.out.format("%d ", arr[i][j]);
            }
            System.out.format("\n");
        }
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_hello:
                tvHello.setText(baseUtils.HelloEve());
                break;
            case R.id.btn_Link:
                tvLink.setText(baseUtils.LinkEve(etLink1.getText().toString().trim(),
                        etLink2.getText().toString().trim()));
                break;
            case R.id.btn_sum:
                sumArr();
                break;
            case R.id.btn_initArray:
                initArray();
                break;
            default:
                break;
        }
    }
}
