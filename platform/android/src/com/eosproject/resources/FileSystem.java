package com.eosproject.resources;
import java.io.File;

import android.content.Context;
import android.os.Environment;
import com.eosproject.eos.EOSActivity;
import com.eosproject.utils.LOG;


public class FileSystem
{
	/** Функция возвращает путь к каталогу, в который сохраняем файлы.
	 * Создаёт каталог при его отсутствии.
	 * Требует обязательного наличия instance SQActivity, использует его в качестве контекста.
	 * @return полный путь к каталогу, с символом '/' в конце.
	 */
	public static String getResourcesPath()
	{
		final EOSActivity acvitivy = EOSActivity.getInstance();
		if (acvitivy == null)
		{
			LOG.FATAL("EOSActivity не существует!");
			return "";
		}
		final Context context = acvitivy.getApplicationContext();
		return getResourcesPath(context);
	}
	
	/** Функция возвращает путь к каталогу, в который сохраняем файлы.
	 * Создаёт каталог при его отсутствии. 
	 * @param context контекст, используется для получения пути к sd карте.
	 * @return полный путь к каталогу, с символом '/' в конце.
	 */
	public static String getResourcesPath(final Context context)
	{
		String result = "";
		boolean sd_available = false;
		
		if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
			//Формируем путь, чтобы файлы по возможности удалялись при удалении пакета.
			// /Android/data/<package_name>/
			result = Environment.getExternalStorageDirectory().getAbsolutePath();
			result += "/Android/data/";
			result += context.getPackageName();
			result += "/";
			
			//Пытаемся создать каталог, Если не удаётся создать, значит скорее всего нет места на флешке. В таком случае используем внутреннюю память телефона.
			File cache_dir = new File(result);
			if (cache_dir.isDirectory()) {
				sd_available = true;
			} else {
				sd_available = cache_dir.mkdirs();
			}
		}
		
		if (!sd_available) {
			//Если SD-карта недоступна, используем хранилище телефона.
			result = context.getFilesDir().getAbsolutePath();
			if (!result.endsWith("/")) {
				result += "/";
			}
		}
		
		return result;
	}
}