package com.@PROJECT_NAME@;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends Activity {
	private static final String module = "@PROJECT_NAME@";
	public native int startModule(String[] args);

	static {
		System.loadLibrary(module);
	}

	@Override protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		String[] args = { module, "-f" };
		int code = startModule(args);

		if (code != 0) {
			Log.w(module, "Module returns: " + code);
		}
	}
}
