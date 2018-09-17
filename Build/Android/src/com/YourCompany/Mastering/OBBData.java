package com.YourCompany.Mastering;

public class OBBData
{
public static final String AppType = "";

public static class XAPKFile {
public final boolean mIsMain;
public final String mFileVersion;
public final long mFileSize;
XAPKFile(boolean isMain, String fileVersion, long fileSize) {
mIsMain = isMain;
mFileVersion = fileVersion;
mFileSize = fileSize;
}
}

public static final XAPKFile[] xAPKS = {
new XAPKFile(
true, // true signifies a main file
"1", // the version of the APK that the file was uploaded against
0L // the length of the file in bytes
)
};
};

