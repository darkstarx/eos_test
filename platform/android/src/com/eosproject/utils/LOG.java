package com.eosproject.utils;


public class LOG
{
	protected static int INFO = 4;
	protected static int WARN = 5;
	protected static int ERROR = 6;
	protected static int FATAL = 7;


	public static void INFO(String text)
	{
		WRITE(INFO, text);
	}


	public static void INFO_IF(boolean condition, String text)
	{
		LOG_IF(INFO, condition, text);
	}


	public static void WARN(String text)
	{
		WRITE(WARN, text);
	}


	public static void WARN_IF(boolean condition, String text)
	{
		LOG_IF(WARN, condition, text);
	}


	public static void ERROR(String text)
	{
		WRITE(ERROR, text);
	}


	public static void ERROR_IF(boolean condition, String text)
	{
		LOG_IF(ERROR, condition, text);
	}


	public static void FATAL(String text)
	{
		WRITE(FATAL, text);
	}


	public static void FATAL_IF(boolean condition, String text)
	{
		LOG_IF(FATAL, condition, text);
	}


	public static void CHECK(boolean condition, String text_on_error)
	{
		LOG_IF(FATAL, !condition, text_on_error);
	}


	protected static void WRITE(final int level, final String text)
	{
		StackTraceElement caller = new StackTraceElement("JAVA", "", "", 0);

		StackTraceElement[] stackTraceElements = Thread.currentThread().getStackTrace();
		boolean self_found = false;
		for (int n = 0; n < stackTraceElements.length; ++n) {
			caller = stackTraceElements[n];
			if (caller.getFileName().matches("^D?LOG.java$")) {
				if (!self_found) self_found = true;
			} else {
				if (self_found) break;
			}
		}

		String func = caller.getClassName();
		int pos = func.lastIndexOf(".");
		if (pos > 0) func = func.substring(pos + 1);
		func += ":" + caller.getMethodName();
		final String file = caller.getFileName();
		final int line = caller.getLineNumber();

		nativeLog(level, file + ":" + func, line, text == null ? "null" : text);
	}


	protected static void LOG_IF(int level, boolean condition, String text)
	{
		if (condition) WRITE(level, text);
	}


	private static native void nativeLog(int level, String file, int line, String text);
	
	public static native void nativeSetLogDirectoryPath(String path);
	public static native void nativeCreateLogFile();
	public static native void nativeFlushLogFile();
}
